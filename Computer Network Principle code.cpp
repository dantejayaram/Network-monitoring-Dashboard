#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

const char *validUsername = "admin";
const char *validPassword = "admin123";

bool checkUserAccess(const char *username, const char *password) {
    return (strcmp(username, validUsername) == 0 && strcmp(password, validPassword) == 0);
}

void fetchInput() {
    g_print("Fetching system information...\n");
    FILE *osInfo = popen("systeminfo | findstr /B /C:\"OS Name\" /C:\"OS Version\"", "r");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), osInfo) != NULL) {
        g_print("%s", buffer);
    }
    pclose(osInfo);
    FILE *cpuInfo = popen("wmic cpu get caption", "r");
    while (fgets(buffer, sizeof(buffer), cpuInfo) != NULL) {
        g_print("CPU: %s", buffer);
    }
    pclose(cpuInfo);
    FILE *memoryInfo = popen("wmic memorychip get capacity", "r");
    while (fgets(buffer, sizeof(buffer), memoryInfo) != NULL) {
        g_print("Memory: %s", buffer);
    }
    pclose(memoryInfo);
    g_print("Network Status: Connected\n");
}
void displayDeviceStatus() {
    g_print("Displaying device status...\n");
    system("echo 'Router Status:'");
    system("ipconfig"); 
    system("echo 'Server Status:'");
    system("tasklist");  
}
void displayTrafficAnalysis() {
    printf("'Traffic Analysis:'\n");
    system("netstat -an");
    printf("'Bandwidth Usage:'\n");
    system("ping -n 5 google.com"); 
   printf("Identifying Potential Bottlenecks/Anomalies:\n");
FILE *cpuUsageFile = popen("wmic cpu get loadpercentage | findstr /r /v '^$'", "r");
int cpuUsage;
fscanf(cpuUsageFile, "%d", &cpuUsage);
pclose(cpuUsageFile);
printf("CPU Usage: %d\n", cpuUsage);
FILE *diskUsageFile = popen("fsutil volume diskfree C: | findstr /r /v '^$'", "r");
unsigned long long freeBytes, totalBytes;
fscanf(diskUsageFile, "%*s %*s %llu %llu", &freeBytes, &totalBytes);
pclose(diskUsageFile);
double diskUsage = ((totalBytes - freeBytes) / (double)totalBytes) * 100.0;
printf("Disk Usage: %.2fbytes\n", diskUsage);
}
void showPerformanceMetrics() {
    g_print("Showing performance metrics...\n");
    FILE *cpuUsage = popen("wmic cpu get loadpercentage 2>&1", "r");
    if (cpuUsage == NULL) {
        g_print("Error opening CPU usage command.\n");
        return;
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), cpuUsage) != NULL) {
        g_print("CPU Usage: %s", buffer);
    }
    if (ferror(cpuUsage) != 0) {
        g_print("Error reading from CPU usage command.\n");
    }
    if (pclose(cpuUsage) == -1) {
        g_print("Error closing CPU usage command.\n");
    }
    FILE *memoryUsage = popen("wmic os get freephysicalmemory, totalvisiblememorysize | findstr /r /v '^$'", "r");
    double freeMemory, totalMemory;
    fscanf(memoryUsage, "%*s %lf %*s %*s %lf", &freeMemory, &totalMemory);
    fclose(memoryUsage);
    double memoryUsagePercentage = ((totalMemory - freeMemory) / totalMemory) * 100.0;
    g_print("Memory Usage: %.2f%%\n", memoryUsagePercentage);
    FILE *networkThroughput = popen("wmic nic get Name, NetConnectionStatus, Speed 2>&1", "r");
    if (networkThroughput == NULL) {
        g_print("Error opening network throughput command.\n");
    } else {
        while (fgets(buffer, sizeof(buffer), networkThroughput) != NULL) {
            g_print("Network Throughput: %s", buffer);
        }
        if (ferror(networkThroughput) != 0) {
            g_print("Error reading from network throughput command.\n");
        }
        if (pclose(networkThroughput) == -1) {
            g_print("Error closing network throughput command.\n");
        }
    }
    FILE *diskUsage = popen("wmic logicaldisk get Name, FreeSpace, Size 2>&1", "r");
    if (diskUsage == NULL) {
        g_print("Error opening disk usage command.\n");
    } else {
        while (fgets(buffer, sizeof(buffer), diskUsage) != NULL) {
            g_print("Disk Usage: %s", buffer);
        }
        if (ferror(diskUsage) != 0) {
            g_print("Error reading from disk usage command.\n");
        }
        if (pclose(diskUsage) == -1) {
            g_print("Error closing disk usage command.\n");
        }
    }
}
void viewData() {
    g_print("Viewing data...\n");
    g_print("Data Source: User's Home Directory\n");
    system("ls -l ~/");
    const char *directoryPath = "C:\\";
    g_print("Viewing data from directory: %s\n", directoryPath);
    DIR *dir = opendir(directoryPath);
    if (dir) {
        struct dirent *entry;
        struct stat file;
        struct stat fileStat;
        int count = 1;
        while ((entry = readdir(dir)) != NULL) {
            char filePath[256];
            snprintf(filePath, sizeof(filePath), "%s%s", directoryPath, entry->d_name);
            if (stat(filePath, &fileStat) == 0) {
                g_print("%d. %s\n", count, entry->d_name);
                g_print("   Size: %lld bytes\n", (long long)fileStat.st_size);
                g_print("   Last Modified: %s", ctime(&fileStat.st_mtime)); 
                count++;
            }
        }
        closedir(dir);
    } else {
        g_print("Error: Unable to open the directory.\n");
    }
}

void onExitClicked(GtkWidget *widget, gpointer data) {
    g_print("Exiting the program...\n");
    gtk_main_quit(); 
}

void onFetchDataClicked(GtkWidget *widget, gpointer data) {
    fetchInput();
}
void onDisplayStatusClicked(GtkWidget *widget, gpointer data) {
    displayDeviceStatus();
}
void onAnalyzeTrafficClicked(GtkWidget *widget, gpointer data) {
    displayTrafficAnalysis();
}
void onShowMetricsClicked(GtkWidget *widget, gpointer data) {
    showPerformanceMetrics();
}

void onUserAccessClicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkWidget *contentArea;
    GtkWidget *grid;
    GtkWidget *labelUsername, *labelPassword;
    GtkWidget *entryUsername, *entryPassword;
    dialog = gtk_dialog_new_with_buttons("User Access", NULL, GTK_DIALOG_MODAL, "OK", GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, NULL);
    contentArea = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(contentArea), grid);
    labelUsername = gtk_label_new("Username:");
    gtk_grid_attach(GTK_GRID(grid), labelUsername, 0, 0, 1, 1);
    entryUsername = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entryUsername, 1, 0, 1, 1);
    labelPassword = gtk_label_new("Password:");
    gtk_grid_attach(GTK_GRID(grid), labelPassword, 0, 1, 1, 1);
    entryPassword = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entryPassword), FALSE); 
    gtk_grid_attach(GTK_GRID(grid), entryPassword, 1, 1, 1, 1);
    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    const char *enteredUsername = gtk_entry_get_text(GTK_ENTRY(entryUsername));
    const char *enteredPassword = gtk_entry_get_text(GTK_ENTRY(entryPassword));
    if (result == GTK_RESPONSE_OK) {
        if (checkUserAccess(enteredUsername, enteredPassword)) {
            g_print("Access granted!\n");
            viewData();
        } else {
            g_print("Access denied. Invalid username or password.\n");
        }
    }
    gtk_widget_destroy(dialog);
}
int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *fetchButton, *statusButton, *trafficButton, *metricsButton, *dataButton, *accessButton,*exitButton;
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Network Monitoring Dashboard");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    fetchButton = gtk_button_new_with_label("Fetch Data");
    statusButton = gtk_button_new_with_label("Display Status");
    trafficButton = gtk_button_new_with_label("Analyze Traffic");
    metricsButton = gtk_button_new_with_label("Show Metrics");
    dataButton = gtk_button_new_with_label("View Data");
    accessButton = gtk_button_new_with_label("User Access");
    exitButton = gtk_button_new_with_label("Exit");
    g_signal_connect(fetchButton, "clicked", G_CALLBACK(onFetchDataClicked), NULL);
    g_signal_connect(statusButton, "clicked", G_CALLBACK(onDisplayStatusClicked), NULL);
    g_signal_connect(trafficButton, "clicked", G_CALLBACK(onAnalyzeTrafficClicked), NULL);
    g_signal_connect(metricsButton, "clicked", G_CALLBACK(onShowMetricsClicked), NULL);
    g_signal_connect(accessButton, "clicked", G_CALLBACK(onUserAccessClicked), window); // Pass the window as data
    g_signal_connect(exitButton, "clicked", G_CALLBACK(onExitClicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), fetchButton, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), statusButton, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), trafficButton, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), metricsButton, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), exitButton, 1,2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), accessButton, 0, 2, 1, 1);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}

