

typedef struct
{
    __IO uint32_t WriteAddress;
    __IO uint32_t ReadAddress;
    __IO uint32_t ReadOverflow;
    __IO uint32_t StartAddress;
    __IO uint32_t StartOverflow;
} FLASH_FIFO_Typedef;

FLASH_FIFO_Typedef m_Flash_FIFO;
const uint32_t FLASH_SECTER_SIZE = 4096u;
const uint32_t FLASH_SECTERS     = 2048u;

#define M2SADDR(ADDR, SECTER)   \
    (ADDR % SECTER)
#define SECTADDR(ADDR, SECTER)   \
    (ADDR / SECTER)
        
// ��ʼ��
void MDW_Flash_Init(void)
{
    // ��ʼ��, ����FIFO��ַ
    HAL_Config_Read((uint8_t *)&m_Flash_FIFO);
}

// ��Flash��д��x֡����
// uint8_t *pBuffer ������ָ��
// uint16_t usFrameLength һ֡���ݵ��ֽڴ�С
// uint16_t usFrameSize ���ݻ������е�֡����
bool MDW_Flash_Write(const uint8_t *pBuffer, uint16_t usFrameLength, uint16_t usFrameSize)
{
    // ���ȿ���һ֡�����
    if (0x1 == usFrameSize)
    {
        // �ж�ʱ�򳬹�������ַ
        if (M2SADDR(m_Flash_FIFO.WriteAddress, FLASH_SECTER_SIZE) + usFrameLength < FLASH_SECTER_SIZE)
        {
            HAL_Storage_Write(pBuffer, m_Flash_FIFO.WriteAddress, usFrameLength);
            m_Flash_FIFO.WriteAddress += usFrameLength;
            // ���´洢�еı���
            HAL_Config_Write((uint8_t *)&m_Flash_FIFO);
        } else {
            // ��ʱFlash�����е�ʣ���ַ����
            m_Flash_FIFO.WriteAddress = SECTADDR(m_Flash_FIFO.WriteAddress, FLASH_SECTER_SIZE) + FLASH_SECTER_SIZE;
            if (m_Flash_FIFO.WriteAddress >= FLASH_SECTER_SIZE * FLASH_SECTERS)
            {
                m_Flash_FIFO.WriteAddress = 0;
                m_Flash_FIFO.ReadOverflow = 0;
                m_Flash_FIFO.StartOverflow = 0;
            }
            if (m_Flash_FIFO.StartAddress == m_Flash_FIFO.WriteAddress)
            {
                // ���δ��ָ���ַ
                if (m_Flash_FIFO.StartAddress == SECTADDR(m_Flash_FIFO.ReadAddress, FLASH_SECTER_SIZE))
                {
                    m_Flash_FIFO.ReadAddress += FLASH_SECTER_SIZE;
                    if (m_Flash_FIFO.ReadAddress >= FLASH_SECTER_SIZE * FLASH_SECTERS)
                    {
                        m_Flash_FIFO.ReadAddress = 0;
                        m_Flash_FIFO.ReadOverflow = 1;
                    }
                }
                // �����ʼ��ָ���ַ
                m_Flash_FIFO.StartAddress += FLASH_SECTER_SIZE;
                if (m_Flash_FIFO.StartAddress >= FLASH_SECTER_SIZE * FLASH_SECTERS)
                {
                    m_Flash_FIFO.StartAddress = 0;
                    m_Flash_FIFO.StartOverflow = 1;
                }
            }
            HAL_Storage_Erase(m_Flash_FIFO.WriteAddress);
            HAL_Storage_Write(pBuffer, m_Flash_FIFO.WriteAddress, usFrameLength);
            m_Flash_FIFO.WriteAddress += usFrameLength;
            // ���´洢�еı���
            HAL_Config_Write((uint8_t *)&m_Flash_FIFO);
        }
        return true;
    }
    return false;
}

// ��Flash�ж�ȡx֡����
// uint8_t *pBuffer ������ָ��
// uint16_t usFrameLength һ֡���ݵ��ֽڴ�С
// uint16_t usFrameSize ���ݻ������е�֡����
bool MDW_Flash_Read(uint8_t *pBuffer, uint16_t usFrameLength, uint16_t usFrameSize)
{
    if (MDW_Flash_GetFrame(usFrameLength) < usFrameSize)
    {
        return false;
    }
    for (int i = 0; i < usFrameSize; i ++)
    {
        // �ж�ʱ�򳬹�������ַ
        if (M2SADDR(m_Flash_FIFO.ReadAddress, FLASH_SECTER_SIZE) + usFrameLength < FLASH_SECTER_SIZE)
        {
            HAL_Storage_Read(pBuffer, m_Flash_FIFO.ReadAddress, usFrameLength);
            m_Flash_FIFO.ReadAddress += usFrameLength;
        } else {
            // ��ʱFlash�����е�ʣ���ַ����
            m_Flash_FIFO.ReadAddress = SECTADDR(m_Flash_FIFO.ReadAddress, FLASH_SECTER_SIZE) + FLASH_SECTER_SIZE;
            if (m_Flash_FIFO.ReadAddress >= FLASH_SECTER_SIZE * FLASH_SECTERS)
            {
                m_Flash_FIFO.ReadAddress = 0;
                m_Flash_FIFO.ReadOverflow = 1;
            }
        }
    }
    // ���´洢�еı���
    HAL_Config_Write((uint8_t *)&m_Flash_FIFO);
}

uint32_t MDW_Flash_GetFrame(uint16_t usFrameLength)
{
    // �Ƿ����
    if (m_Flash_FIFO.ReadOverflow == 0x1)
    {
        uint32_t sects = (SECTADDR(m_Flash_FIFO.WriteAddress, FLASH_SECTER_SIZE) -
            SECTADDR(m_Flash_FIFO.ReadAddress, FLASH_SECTER_SIZE)) / FLASH_SECTER_SIZE + FLASH_SECTERS;
        uint32_t unused = FLASH_SECTER_SIZE % usFrameLength;
        uint32_t bysize = m_Flash_FIFO.WriteAddress - m_Flash_FIFO.ReadAddress + FLASH_SECTER_SIZE * FLASH_SECTERS;
        return (bysize - sects * unused) / usFrameLength;
    } else {
        uint32_t sects = (SECTADDR(m_Flash_FIFO.WriteAddress, FLASH_SECTER_SIZE) -
            SECTADDR(m_Flash_FIFO.ReadAddress, FLASH_SECTER_SIZE)) / FLASH_SECTER_SIZE;
        uint32_t unused = FLASH_SECTER_SIZE % usFrameLength;
        uint32_t bysize = m_Flash_FIFO.WriteAddress - m_Flash_FIFO.ReadAddress;
        return (bysize - sects * unused) / usFrameLength;
    }
}

