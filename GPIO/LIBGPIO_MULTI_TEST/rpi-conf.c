rtems_gpio_pin_conf led1 = {
 pin_number: 3,
 function: DIGITAL_OUTPUT,
 pull_mode: NO_PULL_RESISTOR,
 interrupt: NULL,
 output_enabled: FALSE,
 logic_invert: FALSE,
 bsp_specific: NULL
};

rtems_gpio_pin_conf led2 = {
 pin_number: 18,
 function: DIGITAL_OUTPUT,
 pull_mode: NO_PULL_RESISTOR,
 interrupt: NULL,
 output_enabled: FALSE,
 logic_invert: FALSE,
 bsp_specific: NULL
};

rtems_gpio_pin_conf sw1 = {
 pin_number: 7,
 function: DIGITAL_INPUT,
 pull_mode: PULL_UP,
 interrupt: NULL,
 output_enabled: FALSE,
 logic_invert: FALSE,
 bsp_specific: NULL
};

rtems_gpio_pin_conf sw2 = {
 pin_number: 2,
 function: DIGITAL_INPUT,
 pull_mode: PULL_UP,
 interrupt: NULL,
 output_enabled: FALSE,
 logic_invert: FALSE,
 bsp_specific: NULL
};
