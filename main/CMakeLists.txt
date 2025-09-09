idf_component_register(
    SRCS 
        "main.c"    
    INCLUDE_DIRS ""
    PRIV_REQUIRES
        u8g2 
        u8g2-hal-esp-idf
)
spiffs_create_partition_image(spiffs ../data FLASH_IN_PROJECT)    