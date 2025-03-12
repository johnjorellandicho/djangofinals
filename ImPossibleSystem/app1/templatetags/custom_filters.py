from django import template

register = template.Library()

@register.filter
def filter_slot_range(slots, range_str):
    """Filter slots by slot number range.
    Usage: {{ slots|filter_slot_range:'0:9' }}
    """
    start, end = map(int, range_str.split(':'))
    return [slot for slot in slots if start <= slot.slot_number <= end]

@register.filter
def filter_available(slots):
    """Filter slots by available status."""
    return [slot for slot in slots if slot.status == 'available']

@register.filter
def count_available_in_range(slots, range_str):
    """Count available slots in a specific range.
    Usage: {{ slots|count_available_in_range:'0:9' }}
    """
    start, end = map(int, range_str.split(':'))
    return sum(1 for slot in slots if start <= slot.slot_number <= end and slot.status == 'available')