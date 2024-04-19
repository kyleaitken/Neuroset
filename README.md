# COMP3004FinalProject
___
### PULL REPO
- `git clone https://github.com/frederickberube0/COMP3004FinalProject`

- If having problems authenticating, try using the github CLI https://cli.github.com/
___
### UML DIAGRAMS | USE CASE DOCUMENTS | TRACEBILITY MATRIX 
* Shared google drive folder containing files: https://drive.google.com/drive/folders/1sLxRDO2KWqIFMNR3Zym54ZfWUgMryNb4?usp=sharing


___
### CONTROLLER [Control Object] [Mediator]
   - **Controller** independantly handles distribution of behaviour for the **MainWindow** [Boundry/Ui Object], **Electrodes** [Entity Object] and **FileManager** [Database Object] of the device
   - **Controller** inherits from QObject for leveraging access to QT signal/slot design tools and the property editor framework for this program
   - **Controller** is instantiated as a member of **MainWindow**, but operates in a separate thread
        - **Controller** thread is initiated in the **MainWindow** constructor with the QThread function:
          ```controller->moveToThread( QThread* ) ```
        - **Controller** thread is properly managed and terminated in the **MainWindow** destructor to ensure clean exit
___



### MAINWINDOW [Boundary/UI Object] [Observer]

   #### MainWindow Design
   - **MainWindow** serves as an **Observer* that responds to notifications from the **Controller** and **Battery**, which operate as the **Subjects*, maintaining a flexible and decoupled architecture in the program design
   - allows the **Controller** and **Battary** to broadcast changes in system state or behavior
   - **MainWindow**, as an **Observer*, updates its view based on these changes without requiring direct interactions
   #### MainWindow QT
   - Combines UI defined in **Ui::MainWindow** with application logic
   - Implementation of button event handling from UI components
   - Created and shown in the _main_ function; relies on **QApplication** for event handling
   - Displays the UI and enters the main event loop via **QApplication**
   #### QApplication [Application Framework (QT Creator)]
   - Initializes and manages GUI application settings
   - Handles command-line inputs and application-wide resources
   - Manages the main event loop for processing events and signals
___
