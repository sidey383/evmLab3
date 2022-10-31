#include <iostream>
#include <libusb-1.0/libusb.h>

using namespace std;

const struct libusb_class_code_name {const enum libusb_class_code code; const char* const value;} usbClassNames[] ={
        {LIBUSB_CLASS_PER_INTERFACE, "PER_INTERFACE"},
        {LIBUSB_CLASS_AUDIO, "AUDIO"},
        {LIBUSB_CLASS_COMM, "COMM"},
        {LIBUSB_CLASS_HID, "HID"},
        {LIBUSB_CLASS_PHYSICAL, "PHYSICAL"},
        {LIBUSB_CLASS_IMAGE, "IMAGE"},
        {LIBUSB_CLASS_PTP, "IMAGE   "},
        {LIBUSB_CLASS_PRINTER, "PRINTER"},
        {LIBUSB_CLASS_MASS_STORAGE, "MASS_STORAGE"},
        {LIBUSB_CLASS_HUB, "HUB"},
        {LIBUSB_CLASS_DATA, "DATA"},
        {LIBUSB_CLASS_SMART_CARD, "SMART_CARD"},
        {LIBUSB_CLASS_CONTENT_SECURITY, "CONTENT_SECURITY"},
        {LIBUSB_CLASS_VIDEO, "VIDEO"},
        {LIBUSB_CLASS_PERSONAL_HEALTHCARE, "PERSONAL_HEALTHCARE"},
        {LIBUSB_CLASS_DIAGNOSTIC_DEVICE, "DIAGNOSTIC_DEVICE"},
        {LIBUSB_CLASS_WIRELESS, "WIRELESS"},
        {LIBUSB_CLASS_APPLICATION, "APPLICATION"},
        {LIBUSB_CLASS_VENDOR_SPEC, "VENDOR_SPEC"},
};

const char* const getUsbClassName(int code) {
    for(int i = 0; i < sizeof(usbClassNames)/sizeof(usbClassNames[0]); i++) {
        if(usbClassNames->code == code) {
            return usbClassNames->value;
        }
    }
    return "UNKNOWN";
}

void print_string_descriptor(libusb_device_handle *handle, struct libusb_device_descriptor desc, int id, string name) {
    unsigned char str[2048];
    if (libusb_get_string_descriptor_ascii(handle, id, str, sizeof(str)) > 0) {
        cout << "\t" << name << ": " << str << "\n";
    } else {
        cout << "\t" << "No " << name << "\n";
    }
}

void print_device(libusb_device *dev) {
    struct libusb_device_descriptor desc;
    libusb_device_handle *handle = NULL;

    if (libusb_get_device_descriptor(dev, &desc) < 0) {
        cout << "failed to get device descriptor" << endl;
        return;
    }

    cout << "Bus "  << (unsigned int) libusb_get_bus_number(dev) << " "
         << "Device " << +libusb_get_device_address(dev) << "\n";
    cout<<"\tProduct id: " << desc.idProduct<<"\n";

    cout<<"\tVendor id: " << desc.idVendor<<"\n";

    cout<<"\tClass: "<<getUsbClassName(desc.bDeviceClass);
    cout<<"\n";

    if (libusb_open(dev, &handle) == LIBUSB_SUCCESS) {
        print_string_descriptor(handle, desc, desc.iProduct, "Product");
        print_string_descriptor(handle, desc, desc.iManufacturer, "Manufacture");
        print_string_descriptor(handle, desc, desc.iSerialNumber, "SerialNumber");
    } else {
        cout << "\tNo additional data\n";
    }

    if (handle) libusb_close(handle);

    cout << endl;
}

int compareDevice(const void* ptr1, const void* ptr2) {
    libusb_device* device1 = *(libusb_device**) ptr1;
    libusb_device* device2 = *(libusb_device**) ptr2;
    uint8_t bus1 = libusb_get_bus_number(device1);
    uint8_t bus2 = libusb_get_bus_number(device2);
    if(bus1 != bus2) {
        if(bus1 > bus2)
            return 1;
        else
            return -1;
    }
    uint8_t address1 = libusb_get_device_address(device1);
    uint8_t address2 = libusb_get_device_address(device2);
    if(address1 != address2) {
        if(address1 > address2)
            return  1;
        else
            return -1;
    }
    return 0;
}

int main() {
    libusb_device **devs;
    ssize_t cnt;
    if (libusb_init(NULL) < 0) {
        cout<<"can't init libusb\n";
        return 0;
    }
    cnt = libusb_get_device_list(NULL, &devs);
    try {
        if (cnt < 0) {
            cout << "no devise\n";
            return 0;
        }
        cout << "found " << cnt << " devices\n";
        qsort(devs, cnt, sizeof(libusb_device *), compareDevice);
        for (int i = 0; i < cnt; i++) print_device(devs[i]);
    } catch (...) {
        cout<<"Error\n";
    }
    libusb_free_device_list(devs, 1);
    libusb_exit(NULL);
}

