### Windows Setup

#### Prerequisites
1. **Install Python 3.x**
   - Download from [python.org](https://www.python.org/downloads/windows/)
   - **IMPORTANT**: During installation, check "Add Python to PATH"
   - Verify installation by opening Command Prompt and running:
     ```cmd
     python --version
     pip --version
     ```

2. **Install Git** (Optional, but recommended)
   - Download from [git-scm.com](https://git-scm.com/download/win)
   - During installation, choose:
     - "Use Git from the Windows Command Prompt"
     - "Use Windows' default console window"

3. **Install Redis**
   - Download Redis for Windows from [Microsoft Archive](https://github.com/microsoftarchive/redis/releases)
   - Or use Windows Subsystem for Linux (WSL) with Redis

#### Project Setup
1. Clone the repository (if using Git):
   ```cmd
   git clone https://github.com/yourusername/ParkSense-Django.git
   cd ParkSense-Django
   ```

2. Create a Virtual Environment:
   ```cmd
   python -m venv env
   ```

3. Activate the Virtual Environment:
   ```cmd
   env\Scripts\activate
   ```

4. Install Dependencies:
   ```cmd
   pip install -r requirements.txt
   ```

5. Configure Database and Redis:
   ```cmd
   python manage.py migrate
   python manage.py populate_parking_cache
   ```

6. Create Superuser:
   ```cmd
   python manage.py createsuperuser
   ```

7. Run Development Server:
   ```cmd
   python manage.py runserver
   ```

#### Troubleshooting
- If you encounter permission issues, run Command Prompt as Administrator
- Ensure all dependencies are compatible with your Python version
- Check that Redis is running before starting the server

#### Optional: Using Windows Subsystem for Linux (WSL)
1. Install WSL from Microsoft Store
2. Install Ubuntu or your preferred Linux distribution
3. Follow Linux setup instructions within WSL