# 🚀 TransitFlow Terminal App

A comprehensive terminal-based public transport management system built in C++ with ASCII-styled UI, featuring both Admin and User modes for complete transport system management.

![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B-orange.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## 🌐 Live Demo

🚀 **[Try TransitFlow Live](https://transitflow.up.railway.app)** - Experience the web version of our terminal app!

*Note: The live demo showcases the core functionality in a web interface, while the full terminal experience is available through local installation.*

## 📋 Table of Contents

- [Live Demo](#live-demo)
- [Overview](#overview)
- [Features](#features)
- [Tech Stack](#tech-stack)
- [Installation](#installation)
- [Usage](#usage)
- [File Structure](#file-structure)
- [User Guide](#user-guide)
- [Admin Guide](#admin-guide)
- [Authentication](#authentication)
- [Contributing](#contributing)
- [Requirements](#requirements)
- [License](#license)

## 🎯 Overview

TransitFlow is a feature-rich terminal application that simulates a real-world public transport management system. It provides separate interfaces for administrators to manage transport services and users to access transport information, see services, and provide feedback.

### Key Highlights
- 🎨 **ASCII Art Interface** - Beautiful terminal UI with colors and animations
- 🔐 **Secure Authentication** - Custom hash function with salt for password security
- 📁 **File-based Storage** - All data stored in organized text files
- ⌨️ **Keyboard Navigation** - Arrow keys and hotkeys for seamless navigation
- 🚫 **No STL Dependencies** - Pure C++ implementation without STL containers

## ✨ Features

### 🔐 Authentication System
- **User Registration & Login** - Secure signup with email validation
- **Admin Access** - Hardcoded secure admin credentials
- **Password Security** - Hidden input with toggle visibility (Tab key)
- **Email Validation** - Custom email format validation
- **Caps Lock Detection** - Toast warnings for caps lock status
- **Secure Hashing** - Custom hash function with random salt

### 👥 User Mode Features
- 🔍 **Transport Search** - Case-insensitive name-based search
- 🕒 **Schedule Viewing** - Formatted display with Name, Route, Timings, Crowd
- 📅 **Time-based Sorting** - Sort by arrival times (AM/PM format)
- 👥 **Crowd-based Sorting** - Sort by crowd levels (Low/Medium/High)
- 🧭 **Route Alternatives** - Find alternate routes using keyword matching
- 🔴 **Live Updates** - Real-time transport information *(Coming Soon)*
- 💺 **Seat Availability** - Check available seats *(Coming Soon)*
- 🚦 **Peak Hour Alerts** - Real-time system clock integration
- 🚩 **Issue Reporting** - Report delays, crowding, and other issues
- 💡 **Special Services** - Request wheelchair access, student discounts
- 🚨 **Emergency Help** - Hotline numbers and safety tips
- 💰 **Fare Information** - Complete pricing for all transport types
- 📝 **Feedback System** - Submit and store user feedback
- ⭐ **Favorite Routes** - Save frequently used routes
- 🔚 **Secure Logout** - Safe session termination

### 🛠️ Admin Mode Features
- ➕ **Service Management** - Add new transport services
- ✏️ **Schedule Updates** - Modify existing transport schedules
- ❌ **Service Deletion** - Remove transport entries
- 👁️ **Complete Overview** - View all transport services
- 📈 **Peak Hour Analysis** - Analyze traffic patterns *(Coming Soon)*
- 📊 **Load Analysis** - Sort by passenger load *(Coming Soon)*
- 🚦 **Traffic Management** - Monitor peak hour traffic
- 🛣️ **Route Optimization** - Recommend route adjustments *(Coming Soon)*
- ⏱️ **Live Delay Updates** - Real-time delay management *(Coming Soon)*
- 📚 **History Tracking** - View transport history *(Coming Soon)*
- 🧾 **Report Generation** - Generate daily transport reports *(Coming Soon)*
- 📢 **Complaint Management** - Review passenger complaints
- 💸 **Fare Management** - Adjust pricing *(Coming Soon)*
- 📜 **System Logs** - View detailed activity logs
- 👥 **User Management** - View all registered users

## 🧠 Tech Stack

| Component | Technology |
|-----------|------------|
| **Language** | C++ (Standard Library Only) |
| **UI Framework** | Terminal-based ASCII with ANSI colors |
| **Platform** | Windows |
| **Headers** | `<iostream>`, `<fstream>`, `<iomanip>`, `<conio.h>`, `<windows.h>` |
| **Storage** | File-based (.txt files) |
| **Authentication** | Custom hash + salt implementation |

## 🚀 Installation

### Prerequisites
- Windows Operating System
- g++ compiler (MinGW recommended)
- Terminal with ANSI escape code support (CMD or PowerShell)
- Bash support for build script

### Quick Start
1. **Clone the repository**
   ```bash
   git clone https://github.com/hanan-bhatti/transitflow.git
   cd transitflow
   ```

2. **Build and run**
   ```bash
   ./build.sh
   ```

3. **Manual compilation** (if build script fails)
   ```bash
   g++ -o src/main.exe src/main.cpp src/auth/*.cpp src/admin/*.cpp src/user/*.cpp src/utils/*.cpp
   cd src && ./main.exe
   ```

4. **Try the live demo**
   🌐 **[Visit TransitFlow Live](https://transitflow.up.railway.app)**

## 📁 File Structure

```
TransitFlow/
│
├── .gitignore                   # Git ignore configuration
├── README.md                    # Project documentation
├── build.sh                     # Build script
│
└── src/
    ├── admin/                   # Admin functionality
    │   ├── admin.cpp            # Admin dashboard implementation
    │   └── admin.h              # Admin headers and declarations
    │
    ├── auth/                    # Authentication system
    │   ├── auth.cpp             # Login/signup implementation
    │   └── auth.h               # Authentication headers
    │
    ├── user/                    # User functionality
    │   ├── user.cpp             # User dashboard implementation
    │   └── user.h               # User headers and declarations
    │
    ├── utils/                   # Utility modules
    │   ├── ascii_titles.txt     # ASCII art titles and graphics
    │   ├── asciiloader.cpp      # ASCII loading animations
    │   ├── asciiloader.h        # ASCII loader headers
    │   ├── consoleUtils.cpp     # Console UI utilities (gotoxy, colors)
    │   ├── consoleUtils.h       # Console utility headers
    │   ├── fileUtils.cpp        # File operations and data handling
    │   ├── fileUtils.h          # File utility headers
    │   ├── filelogger.cpp       # Activity logging system
    │   └── filelogger.h         # Logger headers
    │
    ├── data/                    # Data storage (auto-created)
    │   ├── complaints.txt       # User issue reports
    │   ├── favorites.txt        # Saved favorite routes
    │   ├── reports.txt          # User feedback and reports
    │   ├── transport_data.txt   # Transport service information
    │   └── users.txt            # User credentials (hashed)
    │
    ├── logs/                    # System logging
    │   └── activity.log         # Complete system activity logs
    │
    ├── main.cpp                 # Main application entry point
    └── main.exe                 # Compiled executable
```

## 🎮 Usage

### First Time Setup
1. Launch the application
2. Choose "Sign Up" to create a new account
3. Enter valid email and secure password

### Admin Access
- **Email**: `hanan@transitflow.com`
- **Password**: `[Contact administrator at hannanbhatti2006@gmail.com]`

### Navigation
- **Arrow Keys**: Navigate menus
- **Enter**: Select option
- **Tab**: Toggle password visibility

## 💰 Fare Information

| Transport Type | Fare (PKR) |
|---------------|------------|
| Speedo | 25 |
| Metro | 30 |
| EV | 30 |
| Orange Line | 25-45 (varies by stations 1-27) |

## 🔧 Configuration

### Admin Credentials
The admin credentials are hardcoded for security. To change:
1. Modify the hash in `src/auth/admin_auth.cpp`
2. Update the email in the same file
3. Recompile the application

### Data Files
All data files are automatically created in `src/data/` on first run. You can manually edit these files if needed:
- `users.txt`: User authentication data
- `transport_data.txt`: Transport service information
- `reports.txt`: User feedback and reports

## 🛡️ Security Features

- **Password Hashing**: Custom implementation with salt
- **Hidden Input**: Password masking with toggle option
- **Session Management**: Secure login/logout
- **Input Validation**: Email format and input sanitization
- **Activity Logging**: Complete audit trail

## 🚨 Troubleshooting

### Common Issues

**Build Errors**
```bash
# Ensure g++ is installed
g++ --version

# Check if MinGW is in PATH
where g++

# Try manual compilation
g++ -o src/main.exe src/main.cpp src/auth/*.cpp src/admin/*.cpp src/user/*.cpp src/utils/*.cpp
```

**Terminal Display Issues**
- Use Windows Terminal or PowerShell for best experience
- Ensure terminal supports ANSI escape codes
- Try running as administrator if colors don't appear
- Try to make the text size small with ctrl + '-' to view titles correctly

**File Permission Errors**
- Ensure write permissions in the project directory
- Run terminal as administrator if needed

## 🤝 Contributing

1. Fork the repository from [hanan-bhatti/transitflow](https://github.com/hanan-bhatti/transitflow-cli)
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Guidelines
- Follow existing code style and structure
- Add comments for complex logic
- Test on Windows environment
- Update documentation for new features
- Contact: hannanbhatti2006@gmail.com for any questions

## 📋 Requirements

### System Requirements
- **OS**: Windows 10 or later
- **Compiler**: g++ (MinGW 8.0+)
- **RAM**: 512MB minimum
- **Storage**: 50MB for application and data files

### Development Requirements
- C++ knowledge (Standard Library)
- Windows API familiarity (`<conio.h>`, `<windows.h>`)
- Terminal/Console application experience

## 🔮 Future Enhancements

- [ ] Real-time GPS tracking integration
- [ ] Mobile app companion
- [ ] Database integration (SQLite)
- [ ] Web dashboard for administrators
- [ ] Multi-language support
- [ ] Payment gateway integration
- [ ] SMS/Email notifications
- [ ] Route optimization algorithms

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**Hanan Bhatti**
- 🌐 GitHub: [@hanan-bhatti](https://github.com/hanan-bhatti)
- 📧 Email: hannanbhatti2006@gmail.com
- 🚀 Live Demo: [TransitFlow Live](https://transitflow.up.railway.app)

---

## 🔗 Links

- **🌐 Live Application**: [transitflow.up.railway.app](https://transitflow.up.railway.app)
- **📁 Source Code**: [github.com/hanan-bhatti/transitflow](https://github.com/hanan-bhatti/transitflow-cli)
- **📧 Contact**: hannanbhatti2006@gmail.com

## 🙏 Acknowledgments

- Windows Console API documentation
- ASCII art community for UI inspiration
- Beta testers and contributors

---

⭐ **Star this repository if you find it helpful!**

📧 **Questions?** Open an issue or contact the developer directly.