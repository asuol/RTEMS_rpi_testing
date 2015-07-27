#define led1_pin 3
#define led2_pin 18
#define sw1_pin 7
#define sw2_pin 2

const rtems_gpio_pin_conf inputs[2] =
  {
    {
    pin_number: sw1_pin,
    function: DIGITAL_INPUT,
    pull_mode: PULL_UP,
    interrupt: NULL,
    output_enabled: FALSE,
    logic_invert: FALSE,
    bsp_specific: NULL
    },
    {
    pin_number: sw2_pin,
    function: DIGITAL_INPUT,
    pull_mode: PULL_UP,
    interrupt: NULL,
    output_enabled: FALSE,
    logic_invert: FALSE,
    bsp_specific: NULL
    }
  };

const rtems_gpio_pin_conf outputs[2] =
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
    }
  };

rtems_gpio_group_definition test_group = {
  .digital_inputs = inputs,
  .input_count = 2,
  .digital_outputs = outputs,
  .output_count = 2,
  .bsp_specifics = NULL,
  .bsp_specific_pin_count = 0
};


