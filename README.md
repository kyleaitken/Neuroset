# COMP3004FinalProject
___
### PULL REPO
- `git clone https://github.com/frederickberube0/COMP3004FinalProject`

- If having problems authenticating, try using the github CLI https://cli.github.com/
___
### HOW TO RUN
1. Pull the repo
2. In QT Creator, click open project...
3. Select the .pro file found in the neuroset subfolder of the repo
4. Build & run the project by clicking the green triangle
   
___
### UML DIAGRAMS | USE CASE DOCUMENTS | TRACEBILITY MATRIX 
* Shared google drive folder containing files: https://drive.google.com/drive/folders/1sLxRDO2KWqIFMNR3Zym54ZfWUgMryNb4?usp=sharing


___
### CONTROLLER [Control Object] [Mediator]
   - **Controller** independantly handles distribution of behaviour for the **MainWindow** [Boundry/Ui Object], each **Electrode** [Entity Object] and the **FileManager** [Database Object] of the device
   - **Controller** inherits from QObject for leveraging access to QT signal/slot design tools and the property editor framework for this program
   - **Electrode** is instantiated as a member of **MainWindow**, but operates in a separate thread
        - **Controller** thread is initiated in the **MainWindow** constructor with the QThread function:
          ```controller->moveToThread( QThread* ) ```
        - **Controller** thread is properly managed and terminated in the **MainWindow** destructor to ensure clean exit
    
#### CONTROLLER MEMBERS
**Array of Electrode [Entity Object] pointers**
   - Each **Electrode** is instantiated as a member of ****Controller****, but operates in a separate thread
        - **Electrode** thread is initiated in ```Controller::setupElectrodes()``` with the QThread function:
          ```electrode->moveToThread( QThread* ) ```
        - **Electrode** thread is properly managed and terminated in Controller Deconstructor destructor to ensure clean exit

**FileManager [Database Object]**
   - FileManager manages the storage of session data to text files, serving as a persistent storage mechanism upon program exit
   - Operates under the Controller, which provides session data at the end of a complete treatment session for storage
   - Maintains data in a directory named 'Data Output', created during the first file operation
   - Will not store incomplete session data if the session is interrupted
   - Stores files using the format: **Neuroset_Data_yyyy-MM-dd_HH-mm-ss*
___



### MAINWINDOW [Boundary/UI Object] [Observer]

   #### MAINWINDOW DESIGN
   - **MainWindow** serves as an **Observer* that responds to notifications from the **Controller** and **Battery**, which operate as the **Subjects*, maintaining a flexible and decoupled architecture in the program design
   - allows the **Controller** and **Battery** to broadcast changes in system state or behavior
   - **MainWindow**, as an **Observer*, updates its view based on these changes without requiring direct interactions
   #### MAINWINDOW QT
   - Combines UI defined in **Ui::MainWindow** with application logic
   - Implementation of button event handling from UI components
   - Created and shown in the _main_ function; relies on **QApplication** for event handling
   - Displays the UI and enters the main event loop via **QApplication**
   #### QAPPLICATION [Application Framework (QT Creator)]
   - Initializes and manages GUI application settings
   - Handles command-line inputs and application-wide resources
   - Manages the main event loop for processing events and signals

#### MAINWINDOW MEMBERS
**Controller [Control Object] [Subject]**
   - The same Controller instance, which acts as the program's Mediator, also serves as the Subject within the Observer pattern
   - Controller leverages Qt’s QObject signal/slot mechanism for thread-safe broadcast changes in system state and behavior between multiple threads, enabling real-time data exchange and synchronization for the MainWindow interface

**Battery [Entity Object] [Subject]**
   - **Battery** serves as a critical realtime responsive battery management entity, autoincrementing the level of the device battery when the device is powered on and/or charging 
   - **Battery** status significantly influences device operation in events of a low battery level, empty battery level, and charging event
   - **Battery** continiously broadcasts battery level from **BatteryThread** thread function ```BatteryThread::run()``` to be recieved in the slot ```MainWindow::receiveBatteryPercentage( ... )```
   - **Battery** is instantiated as a member of **MainWindow**, but operates in a separate thread
        - **Battery** thread is initiated in the **MainWindow** constructor using **BatteryThread**:
           - **BatteryThread** inherits from **QThread** for leveraging access to QT optimized multithreading implementations 
           - **BatteryThread** is initiated in MainWindow Constructor with the QThread function:
             ```batterythread->start() ```
        - **BatteryThread** thread and **Battery** is properly managed and terminated in the **MainWindow** destructor to ensure clean exit
___
