/*
  不同 51 单品机的编译器语法也是不尽相同的
 **/



/* SDCC */ //开源的 SDCC 用下划线
# define SBIT(name, addr, bit)  __sbit  __at(addr+bit)                    name
# define SFR(name, addr)        __sfr   __at(addr)                        name
# define SFRX(name, addr)       __xdata volatile unsigned char __at(addr) name
# define SFR16(name, addr)      __sfr16 __at(((addr+1U)<<8) | addr)       name
# define SFR16E(name, fulladdr) __sfr16 __at(fulladdr)                    name
# define SFR32(name, addr)      __sfr32 __at(((addr+3UL)<<24) | ((addr+2UL)<<16) | ((addr+1UL)<<8) | addr) name
# define SFR32E(name, fulladdr) __sfr32 __at(fulladdr)                    name


/* Keil C51 */ //keil 直接用关键字
# define SBIT(name, addr, bit)  sbit  name = addr^bit
# define SFR(name, addr)        sfr   name = addr
# define SFRX(name, addr)       volatile unsigned char xdata name _at_ addr
# define SFR16(name, addr)      sfr16 name = addr
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr)  /* not supported */
# define SFR32E(name, fulladdr) /* not supported */


/* Hi-Tech 8051 */ //Hi-Tech 就是 Multisim 里的那货
# define SBIT(name, addr, bit)  volatile bit           name @ (addr+bit)
# define SFR(name, addr)        volatile unsigned char name @ addr
# define SFRX(name, addr)       volatile far unsigned char name @ addr
# define SFR16(name, addr)      /* not supported */
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr)  /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

