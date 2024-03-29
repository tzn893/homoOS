;defines some macro and some data structure that help us develop in 32 protected mode

;----------------------------------------------------------------------------
; 描述符类型值说明
; 其中:
;       DA_  : Descriptor Attribute
;       D    : 数据段
;       C    : 代码段
;       S    : 系统段
;       R    : 只读
;       RW   : 读写
;       A    : 已访问
;       其它 : 可按照字面意思理解
;----------------------------------------------------------------------------
DA_32           equ 4000h       ; 32 位段
DA_LIMIT_4K     equ 8000h       ; 段界限粒度为4K字节

DA_DPL0         equ 10h         ; 特权级0
DA_DPL1         equ 20h         ; 特权级1
DA_DPL2         equ 40h         ; 特权级2
DA_DPL3         equ 60h         ; 特权级3
;----------------------------------------------------------------------------
; 存储段描述符类型值说明
;----------------------------------------------------------------------------
DA_DR           equ 90h         ; 存在的只读数据段属性值
DA_DRW          equ 92h         ; 存在的可读写的数据段属性值
DA_DRWA         equ 93h         ; 存在的已访问可读写数据段属性值
DA_C            equ 98h         ; 存在的只执行代码段属性值
DA_CR           equ 9Ah         ; 存在的可执行可读代码段属性值
DA_CC0          equ 9Ch         ; 存在的可执行一致代码段属性值
DA_CC0R         equ 9Eh         ; 存在的可执行可读一致代码段属性值
;----------------------------------------------------------------------------
; 系统段描述符类型值说明
;----------------------------------------------------------------------------
DA_LDT          equ 82h         ; 局部描述符表段类型值
DA_TaskGate     equ 85h         ; 任务门类型值
DA_386TSS       equ 89h         ; 可用386任务状态段类型值
DA_386CGate     equ 8Ch         ; 386调用门类型值
DA_386IGate     equ 8Eh         ; 386中断门类型值
DA_386TGate     equ 8Fh         ; 386陷阱门类型值
;----------------------------------------------------------------------------


;----------------------------------------------------------------------------
; 选择子类型值说明
; 其中:
;       SA_  : Selector Attribute

SA_RPL0         equ 0       ; @　
SA_RPL1         equ 1       ; #-> RPL
SA_RPL2         equ 2       ; #
SA_RPL3         equ 3       ; @

SA_TIG          equ 0       ; @-> TI
SA_TIL          equ 4       ; @
;----------------------------------------------------------------------------

;----------------------------------------------------------------------------
; 分页机制使用的常量说明
;----------------------------------------------------------------------------
PG_P            equ 1       ; 页存在属性
PG_RW_R         equ 0       ; R/W 属性位值，读/执行
PG_RW_W         equ 2       ; R/W 属性位值，读/写/执行
PG_US_S         equ 0       ; U/S 属性位值，系统级
PG_US_U         equ 4       ; U/S 属性位值，用户级
;----------------------------------------------------------------------------


; 宏 ------------------------------------------------------------------------------------------------------
;
; 描述符
; usage: Descriptor Base, Limit, Attr
;        Base:  dd
;        Limit: dd (low 20 bits available)
;        Attr:  dw (lower 4 bits of higher byte are always 0)
%macro Descriptor 3
    dw  %2 & 0FFFFh                         ; 段界限1 (2字节)
    dw  %1 & 0FFFFh                         ; 段基址1 (2字节)
    db  (%1 >> 16) & 0FFh                   ; 段基址2 (1字节)
    dw  ((%2 >> 8) & 0F00h) | (%3 & 0F0FFh) ; 属性1 + 段界限2 + 属性2 (2字节)
    db  (%1 >> 24) & 0FFh	            	; 段基址 3 (1 字节)
%endmacro ; 共 8 字节
;
; 门
; usage: Gate Selector, Offset, DCount, Attr
;        Selector:  dw
;        Offset:    dd
;        DCount:    db
;        Attr:      db
%macro Gate 4
	dw	(%2 & 0FFFFh)				        ; 偏移 1				(2 字节)
	dw	%1					                ; 选择子				(2 字节)
	dw	(%3 & 1Fh) | ((%4 << 8) & 0FF00h)	; 属性					(2 字节)
	dw	((%2 >> 16) & 0FFFFh)		        ; 偏移 2				(2 字节)
%endmacro ; 共 8 字节
