#include <zephyr/kernel.h>
#include <nrfx.h>

#if NRFX_CHECK(NRFX_TEMP_ENABLED)

#include "nrfx_temp.h"

#if !defined(USE_WORKAROUND_FOR_TEMP_OFFSET_ANOMALY) && defined(NRF51)
// Enable workaround for nRF51 series anomaly 28
// (TEMP: Temperature offset value has to be manually loaded to the TEMP module).
#define USE_WORKAROUND_FOR_TEMP_OFFSET_ANOMALY 1
#endif

/** @brief Time of one check attempt.*/
#define NRFX_TEMP_TIME_US 4

/** @brief Maximum attempts to check whether conversion passed.*/
#define NRFX_TEMP_ATTEMPTS 100

/** @brief Internal state of TEMP driver. */
static nrfx_drv_state_t m_temp_state = NRFX_DRV_STATE_UNINITIALIZED;

/** @brief Pointer to handler to be called from interrupt routine. */
static nrfx_temp_data_handler_t m_data_handler;

nrfx_err_t nrfx_temp_init(nrfx_temp_config_t const *p_config, nrfx_temp_data_handler_t handler)
{
    NRFX_ASSERT(p_config);

    if (m_temp_state != NRFX_DRV_STATE_UNINITIALIZED)
    {
        return NRFX_ERROR_ALREADY;
    }

#if NRFX_CHECK(USE_WORKAROUND_FOR_TEMP_OFFSET_ANOMALY)
    *(uint32_t volatile *)0x4000C504 = 0;
#endif

#if NRFY_TEMP_HAS_CALIBRATION && defined(FICR_TRIM_GLOBAL_TEMP_CALIB_VALUE_Msk)
    nrfy_temp_calibration_coeff_set(NRF_TEMP, NRF_FICR->TRIM.GLOBAL.TEMP.CALIB);
#endif

    m_data_handler = handler;

    if (m_data_handler)
    {
        nrfy_temp_int_init(NRF_TEMP, 0, p_config->interrupt_priority, true);
    }

    m_temp_state = NRFX_DRV_STATE_INITIALIZED;
    return NRFX_SUCCESS;
}

void nrfx_temp_uninit(void)
{
    NRFX_ASSERT(m_temp_state == NRFX_DRV_STATE_INITIALIZED);

    nrfy_temp_task_trigger(NRF_TEMP, NRF_TEMP_TASK_STOP);
    if (m_data_handler)
    {
        nrfy_temp_int_disable(NRF_TEMP, NRF_TEMP_INT_DATARDY_MASK);
        nrfy_temp_int_uninit(NRF_TEMP);
    }

    m_temp_state = NRFX_DRV_STATE_UNINITIALIZED;
}

bool nrfx_temp_init_check(void)
{
    return (m_temp_state != NRFX_DRV_STATE_UNINITIALIZED);
}

int32_t nrfx_temp_calculate(int32_t raw_temperature)
{
    /* Raw temperature is a 2's complement signed value. Moreover, it is represented
     * by 0.25[C] intervals, so division by 4 is needed. To preserve
     * fractional part, raw value is multiplied by 100 before division.*/
    int32_t temperature_celsius = (raw_temperature * 100) / 4;
    printk("Temperature in Celsius: %d\n", temperature_celsius);

    // Convert raw temperature from Celsius to Kelvin
    int32_t temperature_kelvin = temperature_celsius + 27315;
    printk("Temperature in Kelvin: %d\n", temperature_kelvin);
    
    return temperature_kelvin;
}

nrfx_err_t nrfx_temp_measure(void)
{
    NRFX_ASSERT(m_temp_state == NRFX_DRV_STATE_INITIALIZED);

    nrfx_err_t result = NRFX_SUCCESS;

    nrfy_temp_event_clear(NRF_TEMP, NRF_TEMP_EVENT_DATARDY);
    nrfy_temp_task_trigger(NRF_TEMP, NRF_TEMP_TASK_START);

    if (!m_data_handler)
    {
        bool ev_result;
        NRFX_WAIT_FOR(nrfy_temp_event_check(NRF_TEMP, NRF_TEMP_EVENT_DATARDY),
                      NRFX_TEMP_ATTEMPTS,
                      NRFX_TEMP_TIME_US,
                      ev_result);
        if (!ev_result)
        {
            result = NRFX_ERROR_INTERNAL;
        }
        else
        {
            nrfy_temp_event_clear(NRF_TEMP, NRF_TEMP_EVENT_DATARDY);
        }
        nrfy_temp_task_trigger(NRF_TEMP, NRF_TEMP_TASK_STOP);
    }

    return result;
}

void nrfx_temp_irq_handler(void)
{
    NRFX_ASSERT(m_data_handler);

    nrf_temp_task_trigger(NRF_TEMP, NRF_TEMP_TASK_STOP);
    nrf_temp_event_clear(NRF_TEMP, NRF_TEMP_EVENT_DATARDY);

    int32_t raw_temp = nrfx_temp_result_get();
    m_data_handler(raw_temp);
}

#endif // NRFX_CHECK(NRFX_TEMP_ENABLED)
