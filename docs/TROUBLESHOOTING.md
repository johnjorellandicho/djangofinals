# ParkSense Troubleshooting Guide

## Table of Contents
1. [Installation Issues](#installation-issues)
2. [Database Issues](#database-issues)
3. [Authentication Issues](#authentication-issues)
4. [Performance Issues](#performance-issues)
5. [Sensor Integration Issues](#sensor-integration-issues)

## Installation Issues

### Virtual Environment Problems

#### Error: "virtualenv is not recognized as a command"
**Solution:**
```bash
pip install virtualenv
```

#### Error: "pip not found"
**Solution:**
```bash
python -m ensurepip --default-pip
```

### Dependency Installation Failures

#### Error: "Microsoft Visual C++ 14.0 is required"
**Solution:**
1. Download and install Microsoft Build Tools
2. Restart your computer
3. Try installation again

#### Error: "Could not find a version that satisfies the requirement"
**Solution:**
1. Update pip: `pip install --upgrade pip`
2. Check if package is available for your Python version
3. Try installing from wheel file if available

## Database Issues

### Connection Problems

#### Error: "MongoDB Connection Failed"
**Solution:**
1. Check MongoDB service status:
   ```bash
   sudo systemctl status mongod
   ```
2. Start MongoDB if not running:
   ```bash
   sudo systemctl start mongod
   ```
3. Verify database connection:
   ```bash
   mongosh
   use parksense_db
   ```

### MongoDB-Specific Issues

#### Error: "djongo.database.DatabaseError"
**Solution:**
1. Verify MongoDB version compatibility (MongoDB 4.0+ required)
2. Check MongoDB connection string format
3. Ensure MongoDB service is running with authentication properly configured

#### Error: "django.db.utils.OperationalError: cannot connect to MongoDB"
**Solution:**
1. Verify MongoDB is running on the default port (27017)
2. Check firewall settings
3. Ensure MongoDB authentication credentials are correct
4. Try connecting with MongoDB Compass to verify connection

#### Error: "pymongo.errors.ServerSelectionTimeoutError"
**Solution:**
1. Check if MongoDB is running on the specified host/port
2. Verify network connectivity
3. Check MongoDB configuration file:
   ```bash
   cat /etc/mongod.conf
   ```
4. Ensure bindIp setting includes your application's host

### Migration Issues

#### Error: "Collection already exists"
**Solution:**
1. Backup your data
2. Reset migrations:
   ```bash
   find . -path "*/migrations/*.py" -not -name "__init__.py" -delete
   find . -path "*/migrations/*.pyc" -delete
   python manage.py makemigrations
   python manage.py migrate --fake-initial
   ```

#### Error: "Conflicting migrations"
**Solution:**
1. Clear migration history:
   ```bash
   python manage.py migrate --fake app_name zero
   ```
2. Remove migration files
3. Create fresh migrations

## Authentication Issues

### Login Problems

#### Users Cannot Log In
1. Check user exists in database:
   ```python
   python manage.py shell
   from django.contrib.auth.models import User
   User.objects.filter(username='username').exists()
   ```
2. Reset password if necessary:
   ```bash
   python manage.py changepassword username
   ```

#### Session Issues
1. Clear browser cookies and cache
2. Check session configuration in settings.py
3. Verify SESSION_COOKIE_AGE value

### Permission Issues

#### Error: "Permission Denied"
1. Check user groups and permissions in Django admin
2. Verify view decorators
3. Check template conditions for superuser checks

## Performance Issues

### Slow Page Load Times

#### Database Query Optimization
1. Use Django Debug Toolbar to identify slow queries
2. Add appropriate indexes:
   ```python
   class Meta:
       indexes = [
           models.Index(fields=['field_name']),
       ]
   ```
3. Implement caching:
   ```python
   from django.core.cache import cache
   
   # Cache query results
   cache.set('key', value, timeout=3600)
   ```

#### Static Files
1. Enable compression:
   ```python
   STATICFILES_STORAGE = 'django.contrib.staticfiles.storage.ManifestStaticFilesStorage'
   ```
2. Use CDN for static files
3. Implement browser caching

## Sensor Integration Issues

### Sensor Communication

#### Error: "Sensor not responding"
1. Check physical connections
2. Verify sensor power supply
3. Test sensor with diagnostic tools:
   ```python
   python manage.py test_sensor --id SENSOR_1
   ```

#### Error: "Invalid sensor reading"
1. Check sensor calibration
2. Verify data format
3. Monitor sensor logs:
   ```bash
   tail -f /var/log/parksense/sensor.log
   ```

### Data Synchronization

#### Real-time Updates Not Working
1. Check WebSocket connection
2. Verify ASGI configuration
3. Test message queue system

## Logging and Debugging

### Enable Detailed Logging
Add to settings.py:
```python
LOGGING = {
    'version': 1,
    'disable_existing_loggers': False,
    'handlers': {
        'file': {
            'level': 'DEBUG',
            'class': 'logging.FileHandler',
            'filename': '/var/log/parksense/debug.log',
        },
    },
    'loggers': {
        'django': {
            'handlers': ['file'],
            'level': 'DEBUG',
            'propagate': True,
        },
    },
}
```

### Debug Tools
1. Install Django Debug Toolbar:
   ```bash
   pip install django-debug-toolbar
   ```
2. Add to INSTALLED_APPS
3. Configure middleware

## System Maintenance

### Regular Maintenance Tasks
1. Clean old data:
   ```bash
   python manage.py cleanup_old_data --days 30
   ```
2. Backup database:
   ```bash
   mongodump --db parksense_db --out ./backup
   ```
3. Check system logs:
   ```bash
   python manage.py check_system_health
   ```

### Emergency Recovery
1. Restore from backup:
   ```bash
   mongorestore --db parksense_db ./backup/parksense_db
   ```
2. Reset application state:
   ```bash
   python manage.py reset_app_state
   ```
3. Clear all caches:
   ```bash
   python manage.py clear_cache
   ```
