# Generated by Django 3.2.23 on 2025-02-22 14:00

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('app1', '0005_parkinghistory_and_more'),
    ]

    operations = [
        migrations.AddField(
            model_name='parkingslot',
            name='is_occupied',
            field=models.BooleanField(default=False),
        ),
    ]
