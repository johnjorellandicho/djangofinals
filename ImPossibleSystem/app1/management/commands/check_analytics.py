from django.core.management.base import BaseCommand
from django.utils import timezone
from django.db.models import Avg, Sum
from datetime import datetime, timedelta
from app1.models import ParkingHistory, ParkingSlot

class Command(BaseCommand):
    help = 'Check analytics calculations and display detailed statistics'

    def handle(self, *args, **options):
        self.stdout.write("=== Checking Analytics Calculations ===\n")
        
        # Get today's records
        today = timezone.now().date()
        history_data = ParkingHistory.objects.all()
        
        self.stdout.write(f"Total records: {history_data.count()}")
        
        # Check status distribution
        self.stdout.write("\nStatus Distribution:")
        for status in ['available', 'occupied', 'reserved', 'maintenance']:
            count = history_data.filter(status=status).count()
            self.stdout.write(f"{status}: {count}")
        
        # Check slot statistics
        self.stdout.write("\nSlot Statistics:")
        slots = ParkingSlot.objects.all().order_by('vehicle_type', 'slot_number')[:3]
        
        for slot in slots:
            self.stdout.write(f"\nSlot {slot.slot_number} ({slot.get_vehicle_type_display()}):")
            slot_data = history_data.filter(slot=slot)
            occupied_data = slot_data.filter(status='occupied')
            
            # Calculate statistics
            total_duration = occupied_data.aggregate(Sum('duration'))['duration__sum'] or timedelta()
            total_hours = total_duration.total_seconds() / 3600
            total_count = occupied_data.count()
            
            self.stdout.write(f"- Total records: {slot_data.count()}")
            self.stdout.write(f"- Occupied records: {total_count}")
            self.stdout.write(f"- Total occupied hours: {total_hours:.2f}")
            
            if total_count > 0:
                self.stdout.write(f"- Average duration (hours): {total_hours/total_count:.2f}")
            
            # Sample records
            self.stdout.write("- Recent records:")
            for record in slot_data.order_by('-timestamp')[:3]:
                self.stdout.write(
                    f"  * {record.timestamp.strftime('%Y-%m-%d %H:%M:%S')} - "
                    f"Status: {record.status}, "
                    f"Duration: {record.duration}, "
                    f"Rate: {record.occupancy_rate:.1f}%"
                )
