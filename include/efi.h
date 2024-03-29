#ifndef _EFI_H_
#define _EFI_H_

struct EFI_GUID {
  unsigned int Data1;
  unsigned short Data2;
  unsigned short Data3;
  unsigned char Data4[8];
};

enum EFI_LOCATE_SEARCH_TYPE { AllHandles, ByRegisterNotify, ByProtocol };

struct EFI_MP_SERVICES_PROTOCOL {
  unsigned long long (*GetNumberOfProcessors)(
      struct EFI_MP_SERVICES_PROTOCOL *This,
      unsigned long long *NumberOfProcessors,
      unsigned long long *NumberOfEnabledProcessors);
  unsigned long long (*GetProcessorInfo)(
      struct EFI_MP_SERVICES_PROTOCOL *This, unsigned long long ProcessorNumber,
      struct EFI_PROCESSOR_INFORMATION *ProcessorInfoBuffer);
  unsigned long long (*StartupAllAPs)(
      struct EFI_MP_SERVICES_PROTOCOL *This,
      void (*Procedure)(void *ProcedureArgument), unsigned char SingleThread,
      void *WaitEvent, unsigned long long TimeoutInMicroSeconds,
      void *ProcedureArgument, unsigned long long **FailedCpuList);
  unsigned long long (*StartupThisAP)(
      struct EFI_MP_SERVICES_PROTOCOL *This,
      void (*Procedure)(void *ProcedureArgument),
      unsigned long long ProcessorNumber, void *WaitEvent,
      unsigned long long TimeoutInMicroseconds, void *ProcedureArgument,
      unsigned char *Finished);
  unsigned long long (*SwitchBSP)(struct EFI_MP_SERVICES_PROTOCOL *This,
                                  unsigned long long ProcessorNumber,
                                  unsigned char EnableOldBSP);
  unsigned long long (*EnableDisableAP)(struct EFI_MP_SERVICES_PROTOCOL *This,
                                        unsigned long long ProcessorNumber,
                                        unsigned char EnableAP,
                                        unsigned int *HealthFlag);
  unsigned long long (*WhoAmI)(struct EFI_MP_SERVICES_PROTOCOL *This,
                               unsigned long long *ProcessorNumber);
};

struct EFI_SYSTEM_TABLE {
  char _buf1[44];
  struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    unsigned long long _buf;
    unsigned long long (*ReadKeyStroke)(
        struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This, struct EFI_INPUT_KEY *Key);
    void *WaitForKey;
  } * ConIn;
  unsigned long long _buf2;
  struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    unsigned long long _buf;
    unsigned long long (*OutputString)(
        struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, unsigned short *String);
    unsigned long long (*TestString)(
        struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, unsigned short *String);
    unsigned long long (*QueryMode)(
        struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        unsigned long long ModeNumber, unsigned long long *Columns,
        unsigned long long *Rows);
    unsigned long long (*SetMode)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
                                  unsigned long long ModeNumber);
    unsigned long long (*SetAttribute)(
        struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        unsigned long long Attribute);
    unsigned long long (*ClearScreen)(
        struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
    unsigned long long _buf4[2];
    struct SIMPLE_TEXT_OUTPUT_MODE {
      int MaxMode;
      int Mode;
      int Attribute;
      int CursorColumn;
      int CursorRow;
      unsigned char CursorVisible;
    } * Mode;
  } * ConOut;
  unsigned long long _buf3[2];
  struct EFI_RUNTIME_SERVICES {
    char _buf_rs1[24];

    //
    // Time Services
    //
    unsigned long long _buf_rs2[4];

    //
    // Virtual Memory Services
    //
    unsigned long long _buf_rs3[2];

    //
    // Variable Services
    //
    unsigned long long _buf_rs4[3];

    //
    // Miscellaneous Services
    //
    unsigned long long _buf_rs5;
    void (*ResetSystem)(enum EFI_RESET_TYPE ResetType,
                        unsigned long long ResetStatus,
                        unsigned long long DataSize, void *ResetData);
  } * RuntimeServices;
  struct EFI_BOOT_SERVICES {
    char _buf1[24];

    //
    // Task Priority Services
    //
    unsigned long long _buf2[2];

    //
    // Memory Services
    //
    unsigned long long _buf3[2];
    unsigned long long (*GetMemoryMap)(unsigned long long *MemoryMapSize,
                                       struct EFI_MEMORY_DESCRIPTOR *MemoryMap,
                                       unsigned long long *MapKey,
                                       unsigned long long *DescriptorSize,
                                       unsigned int *DescriptorVersion);
    unsigned long long (*AllocatePool)(enum EFI_MEMORY_TYPE PoolType,
                                       unsigned long long Size, void **Buffer);
    unsigned long long (*FreePool)(void *Buffer);

    //
    // Event & Timer Services
    //
    unsigned long long (*CreateEvent)(unsigned int Type,
                                      unsigned long long NotifyTpl,
                                      void (*NotifyFunction)(void *Event,
                                                             void *Context),
                                      void *NotifyContext, void *Event);
    unsigned long long (*SetTimer)(void *Event, enum EFI_TIMER_DELAY Type,
                                   unsigned long long TriggerTime);
    unsigned long long (*WaitForEvent)(unsigned long long NumberOfEvents,
                                       void **Event, unsigned long long *Index);
    unsigned long long _buf4_2[3];

    //
    // Protocol Handler Services
    //
    unsigned long long _buf5[3];
    unsigned long long (*HandleProtocol)(void *Handle,
                                         struct EFI_GUID *Protocol,
                                         void **Interface);
    unsigned long long _buf5_2[5];

    //
    // Image Services
    //
    unsigned long long (*LoadImage)(unsigned char BootPolicy,
                                    void *ParentImageHandle,
                                    struct EFI_DEVICE_PATH_PROTOCOL *DevicePath,
                                    void *SourceBuffer,
                                    unsigned long long SourceSize,
                                    void **ImageHandle);
    unsigned long long (*StartImage)(void *ImageHandle,
                                     unsigned long long *ExitDataSize,
                                     unsigned short **ExitData);
    unsigned long long _buf6[2];
    unsigned long long (*ExitBootServices)(void *ImageHandle,
                                           unsigned long long MapKey);

    //
    // Miscellaneous Services
    //
    unsigned long long _buf7[2];
    unsigned long long (*SetWatchdogTimer)(unsigned long long Timeout,
                                           unsigned long long WatchdogCode,
                                           unsigned long long DataSize,
                                           unsigned short *WatchdogData);

    //
    // DriverSupport Services
    //
    unsigned long long _buf8[2];

    //
    // Open and Close Protocol Services
    //
    unsigned long long (*OpenProtocol)(void *Handle, struct EFI_GUID *Protocol,
                                       void **Interface, void *AgentHandle,
                                       void *ControllerHandle,
                                       unsigned int Attributes);
    unsigned long long _buf9[2];

    //
    // Library Services
    //
    unsigned long long _buf10;
    unsigned long long (*LocateHandleBuffer)(
        enum EFI_LOCATE_SEARCH_TYPE SearchType, struct EFI_GUID *Protocol,
        void *SearchKey, unsigned long long *NoHandles, void ***Buffer);
    unsigned long long (*LocateProtocol)(struct EFI_GUID *Protocol,
                                         void *Registration, void **Interface);
    unsigned long long _buf10_2[2];

    //
    // 32-bit CRC Services
    //
    unsigned long long _buf11;

    //
    // Miscellaneous Services
    //
    void (*CopyMem)(void *Destination, void *Source, unsigned long long Length);
    void (*SetMem)(void *Buffer, unsigned long long Size, unsigned char Value);
    unsigned long long _buf12;
  } * BootServices;
  unsigned long long NumberOfTableEntries;
  struct EFI_CONFIGURATION_TABLE {
    struct EFI_GUID VendorGuid;
    void *VendorTable;
  } * ConfigurationTable;
};

struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
  unsigned long long Revision;
  unsigned long long (*OpenVolume)(struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,
                                   struct EFI_FILE_PROTOCOL **Root);
};
struct EFI_TIME {
  unsigned short Year;  // 1900 – 99
  unsigned char Month;  // 1 – 
  unsigned char Day;    // 1 – 
  unsigned char Hour;   // 0 – 
  unsigned char Minute; // 0 – 
  unsigned char Second; // 0 – 
  unsigned char Pad1;
  unsigned int Nanosecond; // 0 – 999,999,9
  unsigned short TimeZone; // -1440 to 1440 or 2047
  unsigned char Daylight;
  unsigned char Pad2;
};

struct EFI_FILE_INFO {
  unsigned long long Size;
  unsigned long long FileSize;
  unsigned long long PhysicalSize;
  struct EFI_TIME CreateTime;
  struct EFI_TIME LastAccessTime;
  struct EFI_TIME ModificationTime;
  unsigned long long Attribute;
  unsigned short FileName[];
};

struct EFI_FILE_PROTOCOL {
  unsigned long long _buf;
  unsigned long long (*Open)(struct EFI_FILE_PROTOCOL *This,
                             struct EFI_FILE_PROTOCOL **NewHandle,
                             unsigned short *FileName,
                             unsigned long long OpenMode,
                             unsigned long long Attributes);
  unsigned long long (*Close)(struct EFI_FILE_PROTOCOL *This);
  unsigned long long _buf2;
  unsigned long long (*Read)(struct EFI_FILE_PROTOCOL *This,
                             unsigned long long *BufferSize, void *Buffer);
  unsigned long long (*Write)(struct EFI_FILE_PROTOCOL *This,
                              unsigned long long *BufferSize, void *Buffer);
  unsigned long long _buf3[2];
  unsigned long long (*GetInfo)(struct EFI_FILE_PROTOCOL *This,
                                struct EFI_GUID *InformationType,
                                unsigned long long *BufferSize, void *Buffer);
  unsigned long long _buf4;
  unsigned long long (*Flush)(struct EFI_FILE_PROTOCOL *This);
};

struct EFI_GRAPHICS_OUTPUT_PROTOCOL {
  unsigned long long _buf[3];
  struct EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE {
    unsigned int MaxMode;
    unsigned int Mode;
    struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION {
      unsigned int Version;
      unsigned int HorizontalResolution;
      unsigned int VerticalResolution;
      enum EFI_GRAPHICS_PIXEL_FORMAT {
        PixelRedGreenBlueReserved8BitPerColor,
        PixelBlueGreenRedReserved8BitPerColor,
        PixelBitMask,
        PixelBltOnly,
        PixelFormatMax
      } PixelFormat;
    } * Info;
    unsigned long long SizeOfInfo;
    unsigned long long FrameBufferBase;
    unsigned long long FrameBufferSize;
  } * Mode;
};

extern struct EFI_SYSTEM_TABLE *ST;
extern struct EFI_GUID sfsp_guid;
extern struct EFI_GUID fi_guid;

extern struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
extern struct EFI_MP_SERVICES_PROTOCOL *MSP;

void efi_init(struct EFI_SYSTEM_TABLE *system_table);
void *find_efi_acpi_table(void);
#endif
