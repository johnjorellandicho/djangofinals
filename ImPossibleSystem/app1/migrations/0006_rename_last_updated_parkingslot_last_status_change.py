from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('app1', '0005_parkinghistory_and_more'),
    ]

    operations = [
        migrations.RenameField(
            model_name='parkingslot',
            old_name='last_updated',
            new_name='last_status_change',
        ),
    ]
