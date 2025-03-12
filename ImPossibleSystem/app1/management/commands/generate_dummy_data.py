from django.core.management.base import BaseCommand
from django.utils import timezone
from app1.models import ParkingSlot, ParkingHistory
from datetime import timedelta
import random

class Command(BaseCommand):
    help = 'Generate dummy parking history data for testing'

    def add_arguments(self, parser):
        parser.add_argument(
            '--clear',
            action='store_true',
            help='Clear existing parking history before generating new data',
        )

    def _get_peak_hour_probability(self, hour):
        """Return probability of occupancy based on hour of day"""
        # Morning peak (7-9 AM)
        if 7 <= hour <= 9:
            return 0.8
        # Lunch peak (12-2 PM)
        elif 12 <= hour <= 14:
            return 0.7
        # Evening peak (5-7 PM)
        elif 17 <= hour <= 19:
            return 0.9
        # Late night (11 PM-5 AM)
        elif hour >= 23 or hour <= 5:
            return 0.2
        # Normal hours
        else:
            return 0.5

    def handle(self, *args, **kwargs):
        if kwargs['clear']:
            ParkingHistory.objects.all().delete()
            self.stdout.write(self.style.SUCCESS('Cleared existing parking history'))

        now = timezone.now()
        start_time = now - timedelta(hours=24)
        
        # Get all parking slots
        slots = list(ParkingSlot.objects.all())
        if not slots:
            self.stdout.write(self.style.ERROR('No parking slots found. Please create parking slots first.'))
            return

        # Reset analytics
        for slot in slots:
            slot.last_24h_occupancy_count = 0
            slot.last_24h_occupancy_time = timedelta()
            slot.total_occupancy_count = 0
            slot.total_occupancy_time = timedelta()
            slot.status = 'available'
            slot.occupation_start = None
            slot.save()

        # Generate data for each hour in the last 24 hours
        for hour in range(24):
            current_time = start_time + timedelta(hours=hour)
            probability = self._get_peak_hour_probability(current_time.hour)

            # For each slot, decide if it should be occupied this hour
            for slot in slots:
                if random.random() < probability:
                    # Generate 1-3 occupancies per hour
                    num_occupancies = random.randint(1, 3)
                    remaining_minutes = 60
                    
                    for _ in range(num_occupancies):
                        # Calculate duration ensuring we don't exceed the hour
                        max_duration = min(remaining_minutes, random.randint(10, 40))
                        duration = timedelta(minutes=max_duration)
                        remaining_minutes -= max_duration

                        # Create 'occupied' record
                        occupied_start = current_time
                        occupied_slots = sum(1 for s in slots if random.random() < probability)
                        occupancy_rate = (occupied_slots / len(slots)) * 100

                        ParkingHistory.objects.create(
                            slot=slot,
                            timestamp=occupied_start,
                            status='occupied',
                            duration=duration,
                            occupancy_rate=occupancy_rate,
                            occupied_count=occupied_slots
                        )

                        # Create 'available' record after duration
                        available_time = occupied_start + duration
                        occupied_slots = sum(1 for s in slots if random.random() < probability)
                        occupancy_rate = (occupied_slots / len(slots)) * 100

                        ParkingHistory.objects.create(
                            slot=slot,
                            timestamp=available_time,
                            status='available',
                            duration=timedelta(),  # No duration for available state
                            occupancy_rate=occupancy_rate,
                            occupied_count=occupied_slots
                        )

                        # Update slot analytics
                        slot.total_occupancy_count += 1
                        slot.total_occupancy_time += duration
                        slot.last_24h_occupancy_count += 1
                        slot.last_24h_occupancy_time += duration
                        slot.save()

                        if remaining_minutes < 10:
                            break

        self.stdout.write(
            self.style.SUCCESS(
                f'Successfully generated 24 hours of parking data with realistic patterns'
            )
        )
