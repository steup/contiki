/**
 * @file vendor_boardtypes.h
 *
 * @brief vendor-specific PAL board types for AT91SAM3S4B
 *
 * This header file contains vendor-specific board types
 * based on AT91SAM3S4B.
 *
 * $Id: vendor_boardtypes.h,v 1.1.6.3 2011/07/14 15:48:59 dam Exp $
 *
 * @author
 *      dresden elektronik: http://www.dresden-elektronik.de
 *      Support email: support@dresden-elektronik.de
 */
/*
 * Copyright (c) 2010, dresden elektronik All rights reserved.
 *
 * Licensed under dresden elektronik's Limited License Agreement --> deEULA.txt
 */

/* Prevent double inclusion */
#ifndef VENDOR_BOARDTYPES_H
#define VENDOR_BOARDTYPES_H


/* === Macros =============================================================== */


/* Boards for AT86RF212 */

/**
 * dresden elektronik USB radio dongle deRFusb-13E00 (to be coming soon,
 * see: http://www.dresden-elektronik.de/shop/cat4_33.html?language=en)
 *
 * There exist 3 different versions, which are supported by defining one of
 * the following symbols:
 * - PRE_REV00 (internal usage only)
 * - REV00 (outdated)
 * - REV01 (sales version)
 * By default the most recent (REV01; deRFusb-13E001) is used, nothing has to
 * be defined or changed.
 */
#define deRFusb_13E00                (0x80)

/**
 * dresden elektronik USB radio dongle deRFusb-13E04 (internal release).
 * Equals deRFusb-13E00, please use its PAL layer and define 'REV00'.
 */
#define deRFusb_13E04                (0x81)

/**
 * dresden elektronik USB radio dongle deRFusb-13E06 (to be coming soon,
 * see: http://www.dresden-elektronik.de/shop/cat4_33.html?language=en)
 * Equals deRFusb-13E00 REV01, but has an additional iNAND flash on-board.
 */
#define deRFusb_13E06                (0x82)



/* Boards for AT86RF231 */

/**
 * dresden elektronik USB radio dongle deRFusb-23E00
 * (see: http://www.dresden-elektronik.de/shop/prod146.html?language=en)
 *
 * There exist 3 different versions, which are supported by defining one of
 * the following symbols:
 * - PRE_REV00 (internal usage only)
 * - REV00 (outdated)
 * - REV01 (sales version)
 * By default the most recent (REV01; deRFusb-23E001) is used, nothing has to
 * be defined or changed.
 */
#define deRFusb_23E00               (0x83)

/**
 * dresden elektronik USB radio dongle deRFusb-23E04 (internal release).
 * Equals deRFusb-23E00, please use its PAL layer and define 'REV00'.
 */
#define deRFusb_23E04               (0x84)

/**
 * dresden elektronik USB radio dongle deRFusb-23E06 (to be coming soon,
 * see: http://www.dresden-elektronik.de/shop/cat4_33.html?language=en)
 * Equals deRFusb23E00 REV01, but has an additional iNAND flash on-board.
 */
#define deRFusb_23E06               (0x85)

#define deRFsam3_23M10              (0x86)
#define deRFsam3_13M10              (0x87)


#endif  /* VENDOR_BOARDTYPES_H */

/* EOF */
