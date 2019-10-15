# Náplň cvičenia
- zoznámenie sa s prerušeniami - NVIC
- konfigurácia externých prerušení od GPIO s EXTI

# Prerušenia

### NVIC - Nested Vector Interrupt Controller (stm32F303K8)
- 16 programovateľných úrovní priority prerušenia (4 bity)
<p align="center">
    <img src="https://community.arm.com/cfs-file/__key/communityserver-blogs-components-weblogfiles/00-00-00-21-42/4212.figure_5F00_2_5F00_nested_5F00_interrupt.jpg">
</p>

- 76 maskovateľných vektorov prerušení
<p align="center">
    <img src="https://community.arm.com/cfs-file/__key/communityserver-blogs-components-weblogfiles/00-00-00-21-42/6378.figure_5F00_3_5F00_nvic.jpg">
</p>

- tabulka vektorov prerušení je definovaná v súbore "startup_stm32f303x8.s"


