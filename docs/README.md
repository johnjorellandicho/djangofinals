# ParkSense Documentation

## Table of Contents
1. [Introduction](#introduction)
2. [System Requirements](#system-requirements)
3. [Installation](#installation)
4. [Configuration](#configuration)
5. [User Guide](#user-guide)
6. [Admin Guide](#admin-guide)
7. [API Documentation](#api-documentation)
8. [Troubleshooting](#troubleshooting)
9. [Contributing](#contributing)

## Introduction
ParkSense is a smart parking management system built with Django. It provides real-time monitoring of parking slots, analytics, and maintenance tools for parking facility management.

### Key Features
- Real-time parking slot monitoring
- Vehicle type-specific parking slots (cars and motorcycles)
- Analytics and reporting tools
- Maintenance management
- User role-based access control

## System Requirements
- Python 3.8 or higher
- Django 4.2 or higher
- MongoDB 4.0 or higher
- Modern web browser (Chrome, Firefox, Safari, Edge)

## Installation

### 1. Clone the Repository
```bash
git clone https://github.com/ermandac/ParkSense-Django.git
cd ParkSense-Django
```

### 2. Set Up Virtual Environment
```bash
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate
```

### 3. Install Dependencies
```bash
pip install -r requirements.txt
```

### 4. Database Setup
```bash
python manage.py makemigrations
python manage.py migrate
```

### 5. Create Superuser
```bash
python manage.py createsuperuser
```

### 6. Run Development Server
```bash
python manage.py runserver
```

The application will be available at `http://127.0.0.1:8000`

## Configuration

### Environment Variables
Create a `.env` file in the project root with the following variables:
```env
DEBUG=True
SECRET_KEY=your_secret_key
MONGODB_URL=mongodb://localhost:27017/parksense_db
```

### Database Configuration
Update `ImPossibleSystem/settings.py` with your database settings:
```python
DATABASES = {
    'default': {
        'ENGINE': 'djongo',
        'NAME': 'parksense_db',
        # Optional MongoDB connection settings:
        # 'HOST': 'mongodb://localhost:27017',
        # 'USER': 'your_username',
        # 'PASSWORD': 'your_password',
    }
}
```

## User Guide

### Regular Users

#### Login
1. Navigate to `http://127.0.0.1:8000`
2. Enter your username and password
3. Click "Login"

#### Viewing Parking Status
- The home page displays real-time parking slot status
- Car and motorcycle slots are clearly labeled
- Color coding indicates slot status:
  - Green: Available
  - Red: Occupied
  - Yellow: Maintenance

#### Analytics
Regular users can access:
- Occupancy trends
- Vehicle distribution statistics
- Slot utilization reports
- Export analytics reports

### Tools
Regular users can access:
- Analytics tools for generating reports
- Basic slot viewing functionality

## Admin Guide

### Superuser Features
Superusers have access to additional features:

#### System Tools
- Database management
- System configuration
- User management

#### Maintenance Tools
- Slot status management
- Reset slot status
- Toggle maintenance mode
- Change slot types

### Managing Users
1. Access Django admin at `/admin`
2. Navigate to Users section
3. Create/modify user accounts
4. Assign appropriate permissions

## API Documentation

### Sensor Readings Endpoint
```http
POST /api/sensor-reading/
Content-Type: application/json

{
    "sensor_id": "SENSOR_1",
    "distance": 45.5,
    "is_occupied": true
}
```

### Analytics Endpoint
```http
GET /api/analytics/
```

### Maintenance Logging
```http
POST /api/log-maintenance/
Content-Type: application/json

{
    "slot_number": 1,
    "issue": "Sensor calibration required",
    "notes": "Scheduled maintenance"
}
```

## Troubleshooting

### Common Issues and Solutions

#### Database Connection Issues
```
Error: could not connect to server: Connection refused
```
**Solution:**
1. Check if PostgreSQL service is running
2. Verify database credentials
3. Ensure database exists

#### Migration Issues
```
Error: Dependency on app with no migrations
```
**Solution:**
1. Remove migration files (except __init__.py)
2. Run `python manage.py makemigrations`
3. Run `python manage.py migrate`

#### Static Files Not Loading
**Solution:**
1. Run `python manage.py collectstatic`
2. Check STATIC_ROOT and STATIC_URL in settings.py
3. Ensure debug mode is enabled in development

#### Permission Issues
**Solution:**
1. Check user permissions in Django admin
2. Verify user is authenticated
3. Clear browser cache and cookies

### Debug Mode
To enable debug mode for detailed error messages:
1. Set `DEBUG = True` in settings.py
2. Restart the Django server

### Logs
Application logs are located at:
- Error logs: `/var/log/parksense/error.log`
- Access logs: `/var/log/parksense/access.log`

## Contributing

### Development Setup
1. Fork the repository
2. Create a feature branch
3. Make changes
4. Run tests
5. Submit pull request

### Coding Standards
- Follow PEP 8 guidelines
- Write meaningful commit messages
- Include docstrings and comments
- Add tests for new features

### Testing
```bash
python manage.py test
```

### Reporting Issues
- Use GitHub Issues
- Include steps to reproduce
- Attach relevant logs
- Specify environment details
