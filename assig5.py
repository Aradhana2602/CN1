class Node:
    def __init__(self, customerID, callTime):
        self.customerID = customerID
        self.callTime = callTime
        self.next = None

class CallQueue:
    def __init__(self):
        self.front = None
        self.rear = None

    def addCall(self, customerID, callTime):
        newNode = Node(customerID, callTime)
        if self.front is None:
            self.front = newNode
            self.rear = newNode
        else:
            self.rear.next = newNode
            self.rear = newNode
        print("Call added: CustomerID =", customerID, ", CallTime =", callTime)

    def viewQueue(self):
        if self.front is None:
            print("Queue is empty")
            return
        current = self.front
        print("Queue:")
        while current is not None:
            print("CustomerID:", current.customerID, ", CallTime:", current.callTime)
            current = current.next

    def isQueueEmpty(self):
        if self.front is None:
            return True
        else:
            return False

queue = CallQueue()

while True:
    print("\n1. Add Call\n2. View Queue\n3. Check if Queue is Empty\n4. Exit")
    choice = input("Enter choice: ")

    if choice == '1':
        cid = input("Enter Customer ID: ")
        try:
            ctime = int(input("Enter Call Time (minutes): "))
        except ValueError:
            print("Invalid call time, must be an integer.")
            continue
        queue.addCall(cid, ctime)
    elif choice == '2':
        queue.viewQueue()
    elif choice == '3':
        print("Is queue empty?", queue.isQueueEmpty())
    elif choice == '4':
        break
    else:
        print("Invalid choice, try again.")

