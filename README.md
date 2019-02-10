# Multi-vendor Terminal Management System (MTMS)

## Problem Definition
Merchant networks across all sectors of Retail, Hospitality and Travel are greatly dependent on the speed, flexi​ability and convenience of payment acceptance at point-of-sale (POS) terminals – the electronic device deployed at merchant outlets to accept major credit and debit cards. Globally, the adoption rate for POS devices is rising as customers become more and more accustomed to the convenience and payment flexibility that debit and credit cards provide. In Egypt, almost all banking systems provide the payment services through credit and debit cards using POS terminals. While there are multiple vendors that provide POS terminals, and since it is a closed source industry, each vendor provides the terminal management system for its terminals to control, update and manage those terminals. As a result, each bank deals with more than one vendor has to acquire multiple TMS’s to manage all its terminals which is highly costly, requires support from different vendors and makes the whole management process not non-centralized and not unified.

## Project Description
Due to the reasons mentioned above, the need arose for creating one centralized terminal management system that would help the banking systems manage all their POS terminals regardless of their vendor or model by detecting the type of each POS, mapping each one of them to a specific subroutine to handle it and provide it with all the needed features. This centralized system will run on an application server and communicates with a database server that contains a database with all the needed details about the terminals. The communication channel would be through GPRS using SIM cards which is a feature in almost every POS terminal available. This should result in a high reduction in cost, since each banking system would only need one TMS instead of multiple ones and would only need technical support for one system instead of needing support from multiple vendors which would make the whole process easier to manage and handle.

### Basic Features
1.	Test the state of health of the POS terminal. 
2.	Update the installed software on POS terminal.
3.	Update the hotlist file of stolen or lost Credit/Debit cards.
4.	Perform some housekeeping on the POS terminal.


### Components
1. Embedded C application for the POS running Embedded Linux
2. Android application for the POS running Android OS
3. Web-based application for the TMS interface (HTML, CSS, jQuery, ASP.NET)
4. Server-side code for communication with the TMS (ASP.NET)
5. Database in MS-SQL

## Team Members
1. Ayman Salah - Full Stack Development
2. Ibrahim Hussiny - Android Development
3. Mohamed Asaker - Embedded C Development
4. Mohammad Elhariry - Backend Development and Deployment
