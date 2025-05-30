# Generated by Django 4.1.13 on 2025-02-05 05:11

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('app1', '0002_parkingslot_userprofile_preferences_and_more'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='parkingslot',
            name='analytics',
        ),
        migrations.RemoveField(
            model_name='parkingslot',
            name='maintenance_history',
        ),
        migrations.RemoveField(
            model_name='parkingslot',
            name='metadata',
        ),
        migrations.RemoveField(
            model_name='parkingslot',
            name='sensor_data',
        ),
        migrations.AddField(
            model_name='parkingslot',
            name='current_distance',
            field=models.FloatField(default=150.0),
        ),
    ]
