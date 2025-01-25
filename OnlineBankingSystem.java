import java.util.InputMismatchException;
import java.util.Scanner;
import java.util.regex.Pattern;

// Encapsulation: Class representing a customer
class Customer {
    private String name;
    private String email;

    public Customer(String name, String email) {
        this.name = name;
        this.email = email;
    }

    public String getName() {
        return name;
    }

    public String getEmail() {
        return email;
    }
}

// Encapsulation: Abstract class for Account
abstract class Account {
    protected String accountNumber;
    protected Customer customer;
    protected double balance;

    public Account(String accountNumber, Customer customer) {
        this.accountNumber = accountNumber;
        this.customer = customer;
        this.balance = 0.0;
    }

    public abstract void deposit(double amount);
    public abstract void withdraw(double amount);
    public abstract void displayBalance();
}

// Inheritance: SavingsAccount extending Account
class SavingsAccount extends Account {
    private static final double MAX_INTEREST_RATE = 12.0;  // Interest rate limit
    private double interestRate;

    public SavingsAccount(String accountNumber, Customer customer, double interestRate) {
        super(accountNumber, customer);
        this.interestRate = interestRate;
    }

    @Override
    public void deposit(double amount) {
        if (amount <= 0) {
            System.out.println("Deposit amount must be positive.");
            return;
        }
        balance += amount;
        System.out.println("Deposited: " + amount);
    }

    @Override
    public void withdraw(double amount) {
        if (amount <= 0 || amount > 1000000) {  // Withdrawal limit of 10 lakh
            System.out.println("Withdrawal amount must be between 1 and 10,00,000.");
            return;
        }
        if (amount <= balance) {
            balance -= amount;
            System.out.println("Withdrawn: " + amount);
        } else {
            System.out.println("Insufficient funds.");
        }
    }

    @Override
    public void displayBalance() {
        System.out.println("Savings Account Balance: " + balance);
    }

    public double calculateInterest() {
        return balance * interestRate / 100;
    }
}

// Inheritance: CheckingAccount extending Account
class CheckingAccount extends Account {
    public CheckingAccount(String accountNumber, Customer customer) {
        super(accountNumber, customer);
    }

    @Override
    public void deposit(double amount) {
        if (amount <= 0) {
            System.out.println("Deposit amount must be positive.");
            return;
        }
        balance += amount;
        System.out.println("Deposited: " + amount);
    }

    @Override
    public void withdraw(double amount) {
        if (amount <= 0 || amount > 1000000) {  // Withdrawal limit of 10 lakh
            System.out.println("Withdrawal amount must be between 1 and 10,00,000.");
            return;
        }
        if (amount <= balance) {
            balance -= amount;
            System.out.println("Withdrawn: " + amount);
        } else {
            System.out.println("Insufficient funds.");
        }
    }

    @Override
    public void displayBalance() {
        System.out.println("Checking Account Balance: " + balance);
    }
}

// Abstraction: BankingService class
abstract class BankingService {
    public abstract void performService();
}

// Concrete service for managing loans
class LoanService extends BankingService {
    @Override
    public void performService() {
        System.out.println("Managing loan services...");
    }
}

// Concrete service for bill payments
class BillPaymentService extends BankingService {
    @Override
    public void performService() {
        System.out.println("Managing bill payment services...");
    }
}

// Main class for Online Banking System
public class OnlineBankingSystem {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        try {
            Customer customer = getCustomerDetails(scanner);

            while (true) {
                Account account = createAccount(scanner, customer);

                if (account != null) {
                    runBankingMenu(scanner, account);

                    System.out.print("Do you want to switch accounts? (yes/no): ");
                    scanner.nextLine();  // Clear buffer
                    if (!scanner.nextLine().equalsIgnoreCase("yes")) {
                        break;
                    }
                }
            }
        } catch (Exception e) {
            System.out.println("An unexpected error occurred: " + e.getMessage());
        } finally {
            scanner.close();
        }
    }

    private static Customer getCustomerDetails(Scanner scanner) {
        String name, email;

        while (true) {
            System.out.print("Enter customer name: ");
            name = scanner.nextLine();
            if (isValidName(name)) break;
            System.out.println("Invalid name. No numbers or special characters allowed.");
        }

        while (true) {
            System.out.print("Enter customer email: ");
            email = scanner.nextLine();
            if (isValidEmail(email)) break;
            System.out.println("Invalid email. Please enter a valid address.");
        }

        return new Customer(name, email);
    }

    private static boolean isValidName(String name) {
        return name.matches("^[a-zA-Z\\s]+$");
    }

    private static boolean isValidEmail(String email) {
        String emailRegex = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,6}$";
        return Pattern.matches(emailRegex, email);
    }

    private static Account createAccount(Scanner scanner, Customer customer) {
        System.out.print("Enter account type (1: Savings, 2: Checking): ");
        int accountType = scanner.nextInt();

        switch (accountType) {
            case 1:
                System.out.print("Enter interest rate (max 12%): ");
                double interestRate = scanner.nextDouble();
                if (interestRate > 12 || interestRate < 0) {
                    System.out.println("Invalid interest rate. Must be between 0 and 12.");
                    return null;
                }
                return new SavingsAccount("SA" + System.currentTimeMillis(), customer, interestRate);

            case 2:
                return new CheckingAccount("CA" + System.currentTimeMillis(), customer);

            default:
                System.out.println("Invalid account type.");
                return null;
        }
    }

    private static void runBankingMenu(Scanner scanner, Account account) {
        int choice;

        do {
            System.out.println("\nMenu:");
            System.out.println("1. Deposit");
            System.out.println("2. Withdraw");
            System.out.println("3. Display Balance");
            System.out.println("4. Calculate Interest (Savings only)");
            System.out.println("5. Manage Loans");
            System.out.println("6. Pay Bills");
            System.out.println("0. Exit");

            System.out.print("Select an option: ");
            choice = scanner.nextInt();

            switch (choice) {
                case 1:
                    System.out.print("Enter deposit amount: ");
                    double depositAmount = scanner.nextDouble();
                    account.deposit(depositAmount);
                    break;

                case 2:
                    System.out.print("Enter withdrawal amount: ");
                    double withdrawalAmount = scanner.nextDouble();
                    account.withdraw(withdrawalAmount);
                    break;

                case 3:
                    account.displayBalance();
                    break;

                case 4:
                    if (account instanceof SavingsAccount) {
                        double interest = ((SavingsAccount) account).calculateInterest();
                        System.out.println("Interest Earned: " + interest);
                    } else {
                        System.out.println("Interest calculation available for Savings Account only.");
                    }
                    break;

                case 5:
                    new LoanService().performService();
                    break;

                case 6:
                    new BillPaymentService().performService();
                    break;

                case 0:
                    System.out.println("Exiting...");
                    break;

                default:
                    System.out.println("Invalid option. Try again.");
            }
        } while (choice != 0);
    }
}