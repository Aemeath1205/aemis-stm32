#include "BSP_W25Q64.h"

spi_flash_t g_w25qxx;
static bool g_is_init = false;
const uint8_t dummy = 0x00;

// E-6
static void BSP_W25Qxx_CS_Enable(void)
{
    if (!g_is_init)
        return;
    HAL_GPIO_WritePin(g_w25qxx.gpio_port, g_w25qxx.nss_pin, GPIO_PIN_RESET);
}

static void BSP_W25Qxx_CS_Disable(void)
{
    if (!g_is_init)
        return;
    HAL_GPIO_WritePin(g_w25qxx.gpio_port, g_w25qxx.nss_pin, GPIO_PIN_SET);
}

static bool BSP_W25Qxx_Is_Busy(void)
{
   if(BSP_W25Qxx_Statue1_Reg_1() & W25QXX_STATUS_BUSY) return true;
   return false;
}

static bool BSP_W25Qxx_Is_Well(void)
{
    return BSP_W25Qxx_Statue1_Reg_1() & W25QXX_STATUS_WELL ; 
}

bool BSP_W25Qxx_Init(spi_flash_t *init)
{
    if (init == NULL)
        return g_is_init;
    g_w25qxx.page_size = init->page_size;
    g_w25qxx.sector_size = init->sector_size;
    g_w25qxx.block_size = init->block_size;
    g_w25qxx.mem_total_size = init->mem_total_size;
    g_w25qxx.chip_id = init->chip_id;
    g_w25qxx.hspi = init->hspi;
    g_w25qxx.gpio_port = init->gpio_port;
    g_w25qxx.nss_pin = init->nss_pin;

    g_is_init = true;

    // 获得id并比对
    uint32_t id = BSP_W25Qxx_ID();
    if (g_w25qxx.chip_id != id)
    {
        g_is_init = false;
        printf(">> BSP_W25Qxx_Init error, ID error, set ID is: 0x%x, current ID is 0x%x\n", g_w25qxx.chip_id, id);
        return g_is_init;
    }

    printf(">> BSP_W25Qxx_Init successful, ID IS 0x%x\n", g_w25qxx.chip_id);
    return g_is_init;
}

uint32_t BSP_W25Qxx_ID(void)
{
    if (!g_is_init)
        return 0;

    BSP_W25Qxx_CS_Enable();

    uint8_t cmd = CMD_W25QXX_JEDEC_ID;
    uint8_t id[3] = {0};

    HAL_StatusTypeDef state = HAL_SPI_Transmit(g_w25qxx.hspi, &cmd, sizeof(cmd), HAL_MAX_DELAY);
    if (state != HAL_OK)
    {
        printf(">> BSP_W25Qxx_ID, HAL_SPI_Transmit error, cmd is : 0x%x failed, state: 0x%x\n", cmd, state);
        memset(id, 0, sizeof(id));
        goto END;
    }

    state = HAL_SPI_Receive(g_w25qxx.hspi, id, sizeof(id), HAL_MAX_DELAY);
    if (state != HAL_OK)
    {
        printf(">> BSP_W25Qxx_ID, HAL_SPI_Receive error, cmd is : 0x%x failed, state: 0x%x\n", cmd, state);
        memset(id, 0, sizeof(id));
        goto END;
    }

END:
    BSP_W25Qxx_CS_Disable();

    return ((id[0] << 16) | (id[1] << 8) | id[2]);
}

uint8_t BSP_W25Qxx_Statue1_Reg_1(void)
{
    if (!g_is_init)
        return 0xFF;

    uint8_t cmd = CMD_W25QXX_READ_STATUS_REG1;
    uint8_t status_reg = 0xFF;
    BSP_W25Qxx_CS_Enable();

    HAL_StatusTypeDef state = HAL_SPI_Transmit(g_w25qxx.hspi, &cmd, sizeof(cmd), HAL_MAX_DELAY);
    if (state != HAL_OK)
    {
        printf(">> BSP_W25Qxx_Statue1_Reg Transmit error,cmd is :0x%x , state :0x%x\n", cmd, state);
        goto END;
    }
    state = HAL_SPI_Receive(g_w25qxx.hspi, &status_reg, sizeof(status_reg), HAL_MAX_DELAY);
    if (state != HAL_OK)
    {
        status_reg = 0xFF;
        printf(">> BSP_W25Qxx_Statue1_Reg Receive error,cmd is :0x%x , state :0x%x\n", cmd, state);
        goto END;
    }

END:
    BSP_W25Qxx_CS_Disable();

    return status_reg;
}

void BSP_W25Qxx_Write_Enable(void)
{
    if (!g_is_init)
        return;

    uint8_t cmd = CMD_W25QXX_WRITE_ENABLE;
    BSP_W25Qxx_CS_Enable();

    HAL_StatusTypeDef state = HAL_SPI_Transmit(g_w25qxx.hspi, &cmd, sizeof(cmd), HAL_MAX_DELAY);
    if (state != HAL_OK)
    {
        printf(">> BSP_W25Qxx_Write_Enable Transmit error, cmd is :0x%x , state is :0x%x\n", cmd, state);
    }
    BSP_W25Qxx_CS_Disable();
}

void BSP_W25Qxx_Write_Disable(void)
{
    if (!g_is_init)
        return;

    uint8_t cmd = CMD_W25QXX_WRITE_DISABLE;
    BSP_W25Qxx_CS_Enable();

    HAL_StatusTypeDef state = HAL_SPI_Transmit(g_w25qxx.hspi, &cmd, sizeof(cmd), HAL_MAX_DELAY);
    if (state != HAL_OK)
    {
        printf(">> BSP_W25Qxx_Write_Disable Transmit error, cmd is :0x%x , state is :0x%x\n", cmd, state);
    }
    BSP_W25Qxx_CS_Disable();
}

void BSP_W25Qxx_Wait_For_Write_Complete_V1(void)
{
    if (!g_is_init)
        return;
    printf(">>zpccz\n");
    while ((BSP_W25Qxx_Statue1_Reg_1() & W25QXX_STATUS_BUSY) == 1)
    {
        printf(".");
        HAL_Delay(1000);
    }
    printf("\n>>zpccwc\n");
}

void BSP_W25Qxx_Wait_For_Write_Complete_V2(void)
{
    if (!g_is_init)
        return;
    uint8_t cmd = CMD_W25QXX_READ_STATUS_REG1;
    uint8_t reg1 = 0xFF;
    BSP_W25Qxx_CS_Enable();

    HAL_StatusTypeDef state = HAL_SPI_Transmit(g_w25qxx.hspi, &cmd, sizeof(cmd), HAL_MAX_DELAY);
    if (state != HAL_OK)
    {
        printf(">> BSP_W25Qxx_Wait_For_Write_Complete_V2 Transmit error,cmd is :0x%x , state :0x%x\n", cmd, state);
        goto END;
    }
    do
    {
        HAL_SPI_TransmitReceive(g_w25qxx.hspi, &dummy, &reg1, sizeof(dummy), HAL_MAX_DELAY);
    } while (reg1 & W25QXX_STATUS_BUSY);

END:
    BSP_W25Qxx_CS_Disable();
}

void BSP_W25Qxx_Sector_Erase(uint32_t sector_address)
{
    if(!g_is_init) return;
    if(!BSP_W25Qxx_Is_Well())
    {
        BSP_W25Qxx_Write_Enable();
    }
// BSP_W25Qxx_Write_Enable();
    if(sector_address & SECTOR_ADDRESS_MASK) 
    {
        printf(">> BSP_W25Qxx_Sector_Erase error, address 0x%x not sector aligned\n", sector_address);
        return;
    }
    if(sector_address >= g_w25qxx.mem_total_size)
    {
        printf(">> BSP_W25Qxx_Sector_Erase error, address 0x%x out of range max flash size\n", sector_address);
        return;
    }
    uint8_t cmd[4] = {CMD_W25QXX_SECTOR_ERASE , (sector_address>>16) , (sector_address>>8) , sector_address};
    BSP_W25Qxx_CS_Enable();

    HAL_StatusTypeDef state = HAL_SPI_Transmit(g_w25qxx.hspi, cmd, sizeof(cmd), HAL_MAX_DELAY);
    if (state != HAL_OK)
    {
        printf(">> BSP_W25Qxx_Sector_Erase Transmit error,cmd is :0x%x , state :0x%x\n", cmd[0], state);
        goto END;
    }
    printf(">> erase successful\n");
END:
    BSP_W25Qxx_CS_Disable();

    if(state == HAL_OK)
    BSP_W25Qxx_Wait_For_Write_Complete_V2();
}

void BSP_W25Qxx_Chip_Erase(void)
{
    if(!g_is_init) return  ;
    uint8_t cmd = CMD_W25QXX_CHIP_ERASE;
    if(!BSP_W25Qxx_Is_Well())
    {
        BSP_W25Qxx_Write_Enable();
    }

    BSP_W25Qxx_CS_Enable();
    HAL_StatusTypeDef state = HAL_SPI_Transmit(g_w25qxx.hspi, &cmd, sizeof(cmd), HAL_MAX_DELAY);
    if (state != HAL_OK)
    {
        printf(">> BSP_W25Qxx_Chip_Erase Transmit error,cmd is :0x%x , state :0x%x\n", cmd, state);
    }   
    BSP_W25Qxx_CS_Disable();
    if(state == HAL_OK)
    {
        BSP_W25Qxx_Wait_For_Write_Complete_V1();
    }
}

bool BSP_W25Qxx_Read_Data(uint32_t read_address, uint8_t* rbuffer, uint32_t rsize)
{    
    if(!g_is_init) return false ;
    if(rbuffer == NULL || rsize == 0) return false;
    if(read_address + rsize > g_w25qxx.mem_total_size) return false;

    bool ret = true;
    //1.检查是否busy 2.等待忙完成
    if(BSP_W25Qxx_Is_Busy()) BSP_W25Qxx_Wait_For_Write_Complete_V1();
    uint8_t  cmd[4] = {CMD_W25QXX_READ_DATA,(uint8_t)(read_address>>16),(uint8_t)(read_address>>8),(uint8_t)read_address};
    uint16_t cmdlen = sizeof(cmd) /sizeof(cmd[0]);

    //发送
    BSP_W25Qxx_CS_Enable();
    
    HAL_StatusTypeDef state = HAL_SPI_Transmit(g_w25qxx.hspi, cmd, cmdlen, HAL_MAX_DELAY);
    if(state != HAL_OK)
    {
        ret = false;
        printf(">> BSP_W25Qxx_Read_Data Transmit error,cmd is :0x%x, state :0x%x\n", cmd[0], state);
        goto END;
    }

    state = HAL_SPI_Receive(g_w25qxx.hspi, rbuffer, (uint16_t)rsize, HAL_MAX_DELAY);
    if(state != HAL_OK)
    {
        ret = false;
        printf(">> BSP_W25Qxx_Read_Data Receive error,cmd is :0x%x, state :0x%x\n", cmd[0], state);
        goto END;
    }
    printf(">> BSP_W25Qxx_Read_Data successful,cmd is :0x%x, state :0x%x\n", cmd[0], state);
    
END:
    BSP_W25Qxx_CS_Disable();
    return ret;
}

//页写入
bool BSP_W25Qxx_Page_Program(uint32_t page_address, uint16_t page_inner_offset, uint8_t* wbuffer, uint16_t wsize)
{
    if(!g_is_init) return false ;
    if(wbuffer == NULL || wsize == 0) return false;
    if(page_inner_offset + wsize >g_w25qxx.page_size) return false;
    
    uint32_t write_address = (page_address * g_w25qxx.page_size) + page_inner_offset;
    uint8_t cmd[] = {CMD_W25QXX_PAGE_PROGRAM, (uint8_t)(write_address>>16), (uint8_t)(write_address>>8), (uint8_t)(write_address)};
    uint16_t cmdlen = sizeof(cmd) / sizeof(cmd[0]);
    bool ret = true;
    
    
    if(!BSP_W25Qxx_Is_Well())
    {
        BSP_W25Qxx_Write_Enable();
    }
    // BSP_W25Qxx_Write_Enable();
    BSP_W25Qxx_CS_Enable();
    HAL_StatusTypeDef state = HAL_SPI_Transmit(g_w25qxx.hspi, cmd, cmdlen, HAL_MAX_DELAY);
    if(state != HAL_OK)
    {
        ret = false;
        printf(">> BSP_W25Qxx_Page_Program Transmit1 error,cmd is :0x%x, state :0x%x\n", cmd[0], state);
        goto END;
    }

    state = HAL_SPI_Transmit(g_w25qxx.hspi, wbuffer, wsize, HAL_MAX_DELAY);
    if(state != HAL_OK)
    {
        ret = false;
        printf(">> BSP_W25Qxx_Page_Program Transmit2 error,cmd is :0x%x, state :0x%x\n", cmd[0], state);
        goto END;
    }
    printf(">> BSP_W25Qxx_Page_Program success, cmd is :0x%x, state :0x%x\n", cmd[0], state);
END:
    BSP_W25Qxx_CS_Disable();
    
    if(state == HAL_OK)
    BSP_W25Qxx_Wait_For_Write_Complete_V2();
   
    return ret;
}

bool BSP_W25Qxx_Write_Data(uint32_t write_address, uint8_t* wbuffer, uint32_t wsize)
{
    if(!g_is_init) return false ;
    if(wbuffer == NULL || wsize == 0) return false;
    if(write_address + wsize > g_w25qxx.mem_total_size)
    {
        printf("BSP_W25Qxx_Write_Data error,address out range!\n");
        return false;
    }

    //已经写入的字节数
    uint32_t written_bytes = 0;

    while(written_bytes < wsize)
    {
        uint32_t remaining_data = wsize - written_bytes;
        //计算写入的页号
        uint16_t page_num          = write_address / g_w25qxx.page_size;
        uint16_t page_inner_offset = write_address % g_w25qxx.page_size;  
        //当页剩余空间
        uint16_t remaining_space = g_w25qxx.page_size - page_inner_offset;
        //计算本次写入的数据量
        uint16_t to_written = remaining_data > remaining_space? remaining_space :remaining_data;
        BSP_W25Qxx_Page_Program(page_num, page_inner_offset, &wbuffer[written_bytes], to_written);
        
        written_bytes += to_written;
        write_address += to_written;
    }
    return true;
}
