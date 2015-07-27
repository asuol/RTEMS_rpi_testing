#define led1_pin 3
#define led2_pin 18
#define sw1_pin 7
#define sw2_pin 2

rtems_gpio_interrupt_configuration sw1_interrupt = {
  active_interrupt: BOTH_EDGES,
  handler_flag: UNIQUE_HANDLER,
  threaded_interrupts: true,
  handler: edge_test_1,
  arg: NULL,
  debounce_clock_tick_interval: 0
};

rtems_gpio_interrupt_configuration sw2_interrupt = {
  active_interrupt: BOTH_EDGES,
  handler_flag: UNIQUE_HANDLER,
  threaded_interrupts: true,
  handler: edge_test_2,
  arg: NULL,
  debounce_clock_tick_interval: 0
};

const rtems_gpio_pin_conf test[4] =
  {
    {
    pin_number: led1_pin,
    function: DIGITAL_OUTPUT,
    pull_mode: NO_PULL_RESISTOR,
    interrupt: NULL,
    output_enabled: FALSE,
    logic_invert: FALSE,
    bsp_specific: NULL
    },
    {
    pin_number: led2_pin,
    function: DIGITAL_OUTPUT,
    pull_mode: NO_PULL_RESISTOR,
    interrupt: NULL,
    output_enabled: FALSE,
    logic_invert: FALSE,
    bsp_specific: NULL
    },
    {
    pin_number: sw1_pin,
    function: DIGITAL_INPUT,
    pull_mode: PULL_UP,
    interrupt: &sw1_interrupt,
    output_enabled: FALSE,
    logic_invert: FALSE,
    bsp_specific: NULL
    },
    {
    pin_number: sw2_pin,
    function: DIGITAL_INPUT,
    pull_mode: PULL_UP,
    interrupt: &sw2_interrupt,
    output_enabled: FALSE,
    logic_invert: FALSE,
    bsp_specific: NULL
    }
  };
