import java.util.*;

public class ass5{

    // Convert IP to binary string
    static String ipToBinary(String ip) {
        String[] parts = ip.split("\\.");
        StringBuilder binary = new StringBuilder();
        for (String part : parts) {
            binary.append(String.format("%8s", Integer.toBinaryString(Integer.parseInt(part))).replace(' ', '0')).append(".");
        }
        return binary.toString().substring(0, binary.length() - 1);
    }

    // Convert CIDR to subnet mask
    static String cidrToSubnetMask(int cidr) {
        int mask = 0xffffffff << (32 - cidr);
        int[] parts = new int[4];
        for (int i = 0; i < 4; i++) {
            parts[i] = (mask >> (24 - i * 8)) & 0xff;
        }
        return String.format("%d.%d.%d.%d", parts[0], parts[1], parts[2], parts[3]);
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        // Step 1: Enter base IP
        System.out.print("Enter IP address (e.g., 192.168.1.0): ");
        String ip = sc.nextLine();
        System.out.println("Binary of IP: " + ipToBinary(ip));

        // Step 2: Enter number of subnets
        System.out.print("Enter number of required subnets: ");
        int requiredSubnets = sc.nextInt();

        // Assuming Class C network
        int baseCIDR = 24;
        int borrowedBits = 0;
        while (Math.pow(2, borrowedBits) < requiredSubnets) {
            borrowedBits++;
        }

        int newCIDR = baseCIDR + borrowedBits;
        String newSubnetMask = cidrToSubnetMask(newCIDR);
        int totalSubnets = (int) Math.pow(2, borrowedBits);
        int totalHosts = (int) Math.pow(2, 32 - newCIDR);
        int usableHosts = totalHosts - 2;
        int blockSize = 256 - Integer.parseInt(newSubnetMask.split("\\.")[3]);

        System.out.println("\nBinary of subnet mask: " + ipToBinary(newSubnetMask));
        System.out.println("CIDR Notation: /" + newCIDR);
        System.out.println("New Subnet Mask: " + newSubnetMask);
        System.out.println("Total IPs per subnet: " + totalHosts);
        System.out.println("Usable hosts per subnet: " + usableHosts);
        System.out.println("Number of subnets: " + totalSubnets);

        System.out.println("\n📋 Subnet Table:");
        System.out.printf("%-20s %-20s %-20s %-20s\n", "Start IP (Decimal)", "Start IP (Binary)", "End IP (Decimal)", "End IP (Binary)");

        String[] ipParts = ip.split("\\.");
        int base = Integer.parseInt(ipParts[3]);
        int subnetStart = 0;

        for (int i = 0; i < totalSubnets; i++) {
            int start = subnetStart;
            int end = start + blockSize - 1;

            String startIp = ipParts[0] + "." + ipParts[1] + "." + ipParts[2] + "." + start;
            String endIp = ipParts[0] + "." + ipParts[1] + "." + ipParts[2] + "." + end;

            String startIpBinary = ipToBinary(startIp);
            String endIpBinary = ipToBinary(endIp);

            System.out.printf("%-20s %-20s %-20s %-20s\n", startIp, startIpBinary, endIp, endIpBinary);

            subnetStart += blockSize;
        }

        sc.close();
    }
}
