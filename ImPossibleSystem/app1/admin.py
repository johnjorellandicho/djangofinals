from django.contrib import admin
from .models import ParkingSlot, ParkingHistory

# Register your models here.
@admin.register(ParkingSlot)
class ParkingSlotAdmin(admin.ModelAdmin):
    list_display = ('slot_number', 'vehicle_type', 'status', 'sensor_id', 'last_status_change')
    list_filter = ('vehicle_type', 'status')
    search_fields = ('slot_number', 'sensor_id')
    ordering = ('slot_number',)

@admin.register(ParkingHistory)
class ParkingHistoryAdmin(admin.ModelAdmin):
    list_display = ('slot', 'timestamp', 'status', 'duration', 'occupancy_rate')
    list_filter = ('status', 'timestamp', 'slot')
    search_fields = ('slot_number',)
    ordering = ('-timestamp',)