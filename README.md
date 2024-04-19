# COMP3004FinalProject

To pull repo: `git clone https://github.com/frederickberube0/COMP3004FinalProject`

If having problems authenticating, try using the github CLI https://cli.github.com/

Shared google drive folder for draw.io files: https://drive.google.com/drive/folders/1sLxRDO2KWqIFMNR3Zym54ZfWUgMryNb4?usp=sharing


___
### CONTROLLER [Control Object] [Mediator]
   - **Controller** independantly handles distribution of behaviour for the **MainWindow** [Boundry/Ui Object], **Electrodes** [Entity Object] and **FileManager** [Database Object] of the device
   - **Controller** inherits from QObject for leveraging access to QT signal/slot design tools and the property editor framework for this program
   - **Controller** is instantiated as a member of **MainWindow**
        - thread execution in MainWindow Constructor with QThread function:
          ```controller->moveToThread( QThread* ) ```
        - thread exit in MainWindow Deconstructor
___
