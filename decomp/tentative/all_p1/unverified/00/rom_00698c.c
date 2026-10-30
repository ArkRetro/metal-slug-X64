/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x00698c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

byte candidate_698c(void)

{
  char cVar1;
  char cVar2;
  char cVar3;
  byte bVar4;
  char cVar5;
  byte in_D0b;
  byte bVar6;
  int iVar7;
  undefined2 *puVar8;
  int unaff_A5;

  if (*(char *)(unaff_A5 + 0x192c) != '\0') {
    cVar1 = *(char *)(unaff_A5 + 0x191a);
    cVar2 = *(char *)(unaff_A5 + 0x191c);
    cVar3 = *(char *)(unaff_A5 + 0x191e);
    cVar5 = *(char *)(unaff_A5 + 0x192c) + -1;
    *(char *)(unaff_A5 + 0x192c) = cVar5;
    if (cVar5 == '\0') {
      *(undefined2 *)(unaff_A5 + 0x191a) = *(undefined2 *)(unaff_A5 + 0x1920);
      *(undefined2 *)(unaff_A5 + 0x191c) = *(undefined2 *)(unaff_A5 + 0x1922);
      *(undefined2 *)(unaff_A5 + 0x191e) = *(undefined2 *)(unaff_A5 + 0x1924);
    }
    else {
      *(short *)(unaff_A5 + 0x191a) = *(short *)(unaff_A5 + 0x1926) + *(short *)(unaff_A5 + 0x191a);
      *(short *)(unaff_A5 + 0x191c) = *(short *)(unaff_A5 + 0x1928) + *(short *)(unaff_A5 + 0x191c);
      *(short *)(unaff_A5 + 0x191e) = *(short *)(unaff_A5 + 0x192a) + *(short *)(unaff_A5 + 0x191e);
    }
    in_D0b = cVar3 - *(char *)(unaff_A5 + 0x191e) |
             cVar2 - *(char *)(unaff_A5 + 0x191c) | cVar1 - *(char *)(unaff_A5 + 0x191a);
    if (in_D0b != 0) {
      if ((*(byte *)(unaff_A5 + -0x1707) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1707) = *(byte *)(unaff_A5 + -0x1707) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x16f7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x16f7) = *(byte *)(unaff_A5 + -0x16f7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x16e7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x16e7) = *(byte *)(unaff_A5 + -0x16e7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x16d7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x16d7) = *(byte *)(unaff_A5 + -0x16d7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x16c7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x16c7) = *(byte *)(unaff_A5 + -0x16c7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x16b7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x16b7) = *(byte *)(unaff_A5 + -0x16b7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x16a7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x16a7) = *(byte *)(unaff_A5 + -0x16a7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1697) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1697) = *(byte *)(unaff_A5 + -0x1697) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1687) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1687) = *(byte *)(unaff_A5 + -0x1687) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1677) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1677) = *(byte *)(unaff_A5 + -0x1677) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1667) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1667) = *(byte *)(unaff_A5 + -0x1667) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1657) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1657) = *(byte *)(unaff_A5 + -0x1657) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1647) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1647) = *(byte *)(unaff_A5 + -0x1647) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1637) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1637) = *(byte *)(unaff_A5 + -0x1637) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1627) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1627) = *(byte *)(unaff_A5 + -0x1627) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1617) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1617) = *(byte *)(unaff_A5 + -0x1617) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1607) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1607) = *(byte *)(unaff_A5 + -0x1607) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x15f7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x15f7) = *(byte *)(unaff_A5 + -0x15f7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x15e7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x15e7) = *(byte *)(unaff_A5 + -0x15e7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x15d7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x15d7) = *(byte *)(unaff_A5 + -0x15d7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x15c7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x15c7) = *(byte *)(unaff_A5 + -0x15c7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x15b7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x15b7) = *(byte *)(unaff_A5 + -0x15b7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x15a7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x15a7) = *(byte *)(unaff_A5 + -0x15a7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1597) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1597) = *(byte *)(unaff_A5 + -0x1597) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1587) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1587) = *(byte *)(unaff_A5 + -0x1587) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1577) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1577) = *(byte *)(unaff_A5 + -0x1577) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1567) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1567) = *(byte *)(unaff_A5 + -0x1567) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1557) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1557) = *(byte *)(unaff_A5 + -0x1557) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1547) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1547) = *(byte *)(unaff_A5 + -0x1547) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1537) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1537) = *(byte *)(unaff_A5 + -0x1537) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1527) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1527) = *(byte *)(unaff_A5 + -0x1527) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1517) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1517) = *(byte *)(unaff_A5 + -0x1517) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1507) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1507) = *(byte *)(unaff_A5 + -0x1507) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x14f7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x14f7) = *(byte *)(unaff_A5 + -0x14f7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x14e7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x14e7) = *(byte *)(unaff_A5 + -0x14e7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x14d7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x14d7) = *(byte *)(unaff_A5 + -0x14d7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x14c7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x14c7) = *(byte *)(unaff_A5 + -0x14c7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x14b7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x14b7) = *(byte *)(unaff_A5 + -0x14b7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x14a7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x14a7) = *(byte *)(unaff_A5 + -0x14a7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1497) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1497) = *(byte *)(unaff_A5 + -0x1497) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1487) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1487) = *(byte *)(unaff_A5 + -0x1487) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1477) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1477) = *(byte *)(unaff_A5 + -0x1477) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1467) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1467) = *(byte *)(unaff_A5 + -0x1467) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1457) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1457) = *(byte *)(unaff_A5 + -0x1457) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1447) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1447) = *(byte *)(unaff_A5 + -0x1447) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1437) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1437) = *(byte *)(unaff_A5 + -0x1437) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1427) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1427) = *(byte *)(unaff_A5 + -0x1427) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1417) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1417) = *(byte *)(unaff_A5 + -0x1417) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1407) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1407) = *(byte *)(unaff_A5 + -0x1407) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x13f7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x13f7) = *(byte *)(unaff_A5 + -0x13f7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x13e7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x13e7) = *(byte *)(unaff_A5 + -0x13e7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x13d7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x13d7) = *(byte *)(unaff_A5 + -0x13d7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x13c7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x13c7) = *(byte *)(unaff_A5 + -0x13c7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x13b7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x13b7) = *(byte *)(unaff_A5 + -0x13b7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x13a7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x13a7) = *(byte *)(unaff_A5 + -0x13a7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1397) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1397) = *(byte *)(unaff_A5 + -0x1397) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -4999) & 2) == 0) {
        *(byte *)(unaff_A5 + -4999) = *(byte *)(unaff_A5 + -4999) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1377) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1377) = *(byte *)(unaff_A5 + -0x1377) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1367) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1367) = *(byte *)(unaff_A5 + -0x1367) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1357) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1357) = *(byte *)(unaff_A5 + -0x1357) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1347) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1347) = *(byte *)(unaff_A5 + -0x1347) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1337) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1337) = *(byte *)(unaff_A5 + -0x1337) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1327) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1327) = *(byte *)(unaff_A5 + -0x1327) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1317) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1317) = *(byte *)(unaff_A5 + -0x1317) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1307) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1307) = *(byte *)(unaff_A5 + -0x1307) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x12f7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x12f7) = *(byte *)(unaff_A5 + -0x12f7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x12e7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x12e7) = *(byte *)(unaff_A5 + -0x12e7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x12d7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x12d7) = *(byte *)(unaff_A5 + -0x12d7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x12c7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x12c7) = *(byte *)(unaff_A5 + -0x12c7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x12b7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x12b7) = *(byte *)(unaff_A5 + -0x12b7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x12a7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x12a7) = *(byte *)(unaff_A5 + -0x12a7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1297) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1297) = *(byte *)(unaff_A5 + -0x1297) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1287) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1287) = *(byte *)(unaff_A5 + -0x1287) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1277) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1277) = *(byte *)(unaff_A5 + -0x1277) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1267) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1267) = *(byte *)(unaff_A5 + -0x1267) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1257) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1257) = *(byte *)(unaff_A5 + -0x1257) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1247) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1247) = *(byte *)(unaff_A5 + -0x1247) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1237) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1237) = *(byte *)(unaff_A5 + -0x1237) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1227) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1227) = *(byte *)(unaff_A5 + -0x1227) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1217) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1217) = *(byte *)(unaff_A5 + -0x1217) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1207) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1207) = *(byte *)(unaff_A5 + -0x1207) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x11f7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x11f7) = *(byte *)(unaff_A5 + -0x11f7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x11e7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x11e7) = *(byte *)(unaff_A5 + -0x11e7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x11d7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x11d7) = *(byte *)(unaff_A5 + -0x11d7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x11c7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x11c7) = *(byte *)(unaff_A5 + -0x11c7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x11b7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x11b7) = *(byte *)(unaff_A5 + -0x11b7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x11a7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x11a7) = *(byte *)(unaff_A5 + -0x11a7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1197) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1197) = *(byte *)(unaff_A5 + -0x1197) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1187) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1187) = *(byte *)(unaff_A5 + -0x1187) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1177) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1177) = *(byte *)(unaff_A5 + -0x1177) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1167) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1167) = *(byte *)(unaff_A5 + -0x1167) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1157) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1157) = *(byte *)(unaff_A5 + -0x1157) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1147) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1147) = *(byte *)(unaff_A5 + -0x1147) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1137) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1137) = *(byte *)(unaff_A5 + -0x1137) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1127) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1127) = *(byte *)(unaff_A5 + -0x1127) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1117) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1117) = *(byte *)(unaff_A5 + -0x1117) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1107) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1107) = *(byte *)(unaff_A5 + -0x1107) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x10f7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x10f7) = *(byte *)(unaff_A5 + -0x10f7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x10e7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x10e7) = *(byte *)(unaff_A5 + -0x10e7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x10d7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x10d7) = *(byte *)(unaff_A5 + -0x10d7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x10c7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x10c7) = *(byte *)(unaff_A5 + -0x10c7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x10b7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x10b7) = *(byte *)(unaff_A5 + -0x10b7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x10a7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x10a7) = *(byte *)(unaff_A5 + -0x10a7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1097) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1097) = *(byte *)(unaff_A5 + -0x1097) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1087) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1087) = *(byte *)(unaff_A5 + -0x1087) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1077) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1077) = *(byte *)(unaff_A5 + -0x1077) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1067) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1067) = *(byte *)(unaff_A5 + -0x1067) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1057) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1057) = *(byte *)(unaff_A5 + -0x1057) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1047) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1047) = *(byte *)(unaff_A5 + -0x1047) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1037) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1037) = *(byte *)(unaff_A5 + -0x1037) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1027) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1027) = *(byte *)(unaff_A5 + -0x1027) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1017) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1017) = *(byte *)(unaff_A5 + -0x1017) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x1007) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x1007) = *(byte *)(unaff_A5 + -0x1007) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xff7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xff7) = *(byte *)(unaff_A5 + -0xff7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xfe7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xfe7) = *(byte *)(unaff_A5 + -0xfe7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xfd7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xfd7) = *(byte *)(unaff_A5 + -0xfd7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xfc7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xfc7) = *(byte *)(unaff_A5 + -0xfc7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xfb7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xfb7) = *(byte *)(unaff_A5 + -0xfb7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xfa7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xfa7) = *(byte *)(unaff_A5 + -0xfa7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xf97) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xf97) = *(byte *)(unaff_A5 + -0xf97) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xf87) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xf87) = *(byte *)(unaff_A5 + -0xf87) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xf77) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xf77) = *(byte *)(unaff_A5 + -0xf77) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xf67) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xf67) = *(byte *)(unaff_A5 + -0xf67) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xf57) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xf57) = *(byte *)(unaff_A5 + -0xf57) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xf47) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xf47) = *(byte *)(unaff_A5 + -0xf47) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xf37) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xf37) = *(byte *)(unaff_A5 + -0xf37) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xf27) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xf27) = *(byte *)(unaff_A5 + -0xf27) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xf17) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xf17) = *(byte *)(unaff_A5 + -0xf17) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xf07) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xf07) = *(byte *)(unaff_A5 + -0xf07) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xef7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xef7) = *(byte *)(unaff_A5 + -0xef7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xee7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xee7) = *(byte *)(unaff_A5 + -0xee7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xed7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xed7) = *(byte *)(unaff_A5 + -0xed7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xec7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xec7) = *(byte *)(unaff_A5 + -0xec7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xeb7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xeb7) = *(byte *)(unaff_A5 + -0xeb7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xea7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xea7) = *(byte *)(unaff_A5 + -0xea7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xe97) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xe97) = *(byte *)(unaff_A5 + -0xe97) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xe87) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xe87) = *(byte *)(unaff_A5 + -0xe87) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xe77) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xe77) = *(byte *)(unaff_A5 + -0xe77) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xe67) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xe67) = *(byte *)(unaff_A5 + -0xe67) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xe57) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xe57) = *(byte *)(unaff_A5 + -0xe57) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xe47) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xe47) = *(byte *)(unaff_A5 + -0xe47) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xe37) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xe37) = *(byte *)(unaff_A5 + -0xe37) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xe27) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xe27) = *(byte *)(unaff_A5 + -0xe27) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xe17) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xe17) = *(byte *)(unaff_A5 + -0xe17) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xe07) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xe07) = *(byte *)(unaff_A5 + -0xe07) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xdf7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xdf7) = *(byte *)(unaff_A5 + -0xdf7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xde7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xde7) = *(byte *)(unaff_A5 + -0xde7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xdd7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xdd7) = *(byte *)(unaff_A5 + -0xdd7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xdc7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xdc7) = *(byte *)(unaff_A5 + -0xdc7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xdb7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xdb7) = *(byte *)(unaff_A5 + -0xdb7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xda7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xda7) = *(byte *)(unaff_A5 + -0xda7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xd97) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xd97) = *(byte *)(unaff_A5 + -0xd97) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xd87) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xd87) = *(byte *)(unaff_A5 + -0xd87) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xd77) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xd77) = *(byte *)(unaff_A5 + -0xd77) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xd67) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xd67) = *(byte *)(unaff_A5 + -0xd67) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xd57) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xd57) = *(byte *)(unaff_A5 + -0xd57) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xd47) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xd47) = *(byte *)(unaff_A5 + -0xd47) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xd37) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xd37) = *(byte *)(unaff_A5 + -0xd37) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xd27) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xd27) = *(byte *)(unaff_A5 + -0xd27) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xd17) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xd17) = *(byte *)(unaff_A5 + -0xd17) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xd07) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xd07) = *(byte *)(unaff_A5 + -0xd07) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xcf7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xcf7) = *(byte *)(unaff_A5 + -0xcf7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xce7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xce7) = *(byte *)(unaff_A5 + -0xce7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xcd7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xcd7) = *(byte *)(unaff_A5 + -0xcd7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xcc7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xcc7) = *(byte *)(unaff_A5 + -0xcc7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xcb7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xcb7) = *(byte *)(unaff_A5 + -0xcb7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xca7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xca7) = *(byte *)(unaff_A5 + -0xca7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xc97) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xc97) = *(byte *)(unaff_A5 + -0xc97) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xc87) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xc87) = *(byte *)(unaff_A5 + -0xc87) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xc77) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xc77) = *(byte *)(unaff_A5 + -0xc77) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xc67) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xc67) = *(byte *)(unaff_A5 + -0xc67) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xc57) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xc57) = *(byte *)(unaff_A5 + -0xc57) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xc47) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xc47) = *(byte *)(unaff_A5 + -0xc47) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xc37) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xc37) = *(byte *)(unaff_A5 + -0xc37) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xc27) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xc27) = *(byte *)(unaff_A5 + -0xc27) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xc17) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xc17) = *(byte *)(unaff_A5 + -0xc17) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xc07) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xc07) = *(byte *)(unaff_A5 + -0xc07) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xbf7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xbf7) = *(byte *)(unaff_A5 + -0xbf7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xbe7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xbe7) = *(byte *)(unaff_A5 + -0xbe7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xbd7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xbd7) = *(byte *)(unaff_A5 + -0xbd7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xbc7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xbc7) = *(byte *)(unaff_A5 + -0xbc7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -2999) & 2) == 0) {
        *(byte *)(unaff_A5 + -2999) = *(byte *)(unaff_A5 + -2999) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xba7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xba7) = *(byte *)(unaff_A5 + -0xba7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xb97) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xb97) = *(byte *)(unaff_A5 + -0xb97) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xb87) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xb87) = *(byte *)(unaff_A5 + -0xb87) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xb77) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xb77) = *(byte *)(unaff_A5 + -0xb77) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xb67) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xb67) = *(byte *)(unaff_A5 + -0xb67) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xb57) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xb57) = *(byte *)(unaff_A5 + -0xb57) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xb47) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xb47) = *(byte *)(unaff_A5 + -0xb47) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xb37) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xb37) = *(byte *)(unaff_A5 + -0xb37) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xb27) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xb27) = *(byte *)(unaff_A5 + -0xb27) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xb17) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xb17) = *(byte *)(unaff_A5 + -0xb17) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xb07) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xb07) = *(byte *)(unaff_A5 + -0xb07) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xaf7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xaf7) = *(byte *)(unaff_A5 + -0xaf7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xae7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xae7) = *(byte *)(unaff_A5 + -0xae7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xad7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xad7) = *(byte *)(unaff_A5 + -0xad7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xac7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xac7) = *(byte *)(unaff_A5 + -0xac7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xab7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xab7) = *(byte *)(unaff_A5 + -0xab7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xaa7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xaa7) = *(byte *)(unaff_A5 + -0xaa7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xa97) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xa97) = *(byte *)(unaff_A5 + -0xa97) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xa87) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xa87) = *(byte *)(unaff_A5 + -0xa87) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xa77) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xa77) = *(byte *)(unaff_A5 + -0xa77) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xa67) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xa67) = *(byte *)(unaff_A5 + -0xa67) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xa57) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xa57) = *(byte *)(unaff_A5 + -0xa57) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xa47) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xa47) = *(byte *)(unaff_A5 + -0xa47) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xa37) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xa37) = *(byte *)(unaff_A5 + -0xa37) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xa27) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xa27) = *(byte *)(unaff_A5 + -0xa27) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xa17) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xa17) = *(byte *)(unaff_A5 + -0xa17) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0xa07) & 2) == 0) {
        *(byte *)(unaff_A5 + -0xa07) = *(byte *)(unaff_A5 + -0xa07) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x9f7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x9f7) = *(byte *)(unaff_A5 + -0x9f7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x9e7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x9e7) = *(byte *)(unaff_A5 + -0x9e7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x9d7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x9d7) = *(byte *)(unaff_A5 + -0x9d7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x9c7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x9c7) = *(byte *)(unaff_A5 + -0x9c7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x9b7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x9b7) = *(byte *)(unaff_A5 + -0x9b7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x9a7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x9a7) = *(byte *)(unaff_A5 + -0x9a7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x997) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x997) = *(byte *)(unaff_A5 + -0x997) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x987) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x987) = *(byte *)(unaff_A5 + -0x987) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x977) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x977) = *(byte *)(unaff_A5 + -0x977) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x967) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x967) = *(byte *)(unaff_A5 + -0x967) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x957) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x957) = *(byte *)(unaff_A5 + -0x957) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x947) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x947) = *(byte *)(unaff_A5 + -0x947) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x937) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x937) = *(byte *)(unaff_A5 + -0x937) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x927) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x927) = *(byte *)(unaff_A5 + -0x927) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x917) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x917) = *(byte *)(unaff_A5 + -0x917) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x907) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x907) = *(byte *)(unaff_A5 + -0x907) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x8f7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x8f7) = *(byte *)(unaff_A5 + -0x8f7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x8e7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x8e7) = *(byte *)(unaff_A5 + -0x8e7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x8d7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x8d7) = *(byte *)(unaff_A5 + -0x8d7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x8c7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x8c7) = *(byte *)(unaff_A5 + -0x8c7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x8b7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x8b7) = *(byte *)(unaff_A5 + -0x8b7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x8a7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x8a7) = *(byte *)(unaff_A5 + -0x8a7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x897) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x897) = *(byte *)(unaff_A5 + -0x897) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x887) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x887) = *(byte *)(unaff_A5 + -0x887) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x877) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x877) = *(byte *)(unaff_A5 + -0x877) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x867) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x867) = *(byte *)(unaff_A5 + -0x867) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x857) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x857) = *(byte *)(unaff_A5 + -0x857) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x847) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x847) = *(byte *)(unaff_A5 + -0x847) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x837) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x837) = *(byte *)(unaff_A5 + -0x837) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x827) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x827) = *(byte *)(unaff_A5 + -0x827) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x817) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x817) = *(byte *)(unaff_A5 + -0x817) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x807) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x807) = *(byte *)(unaff_A5 + -0x807) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x7f7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x7f7) = *(byte *)(unaff_A5 + -0x7f7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x7e7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x7e7) = *(byte *)(unaff_A5 + -0x7e7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x7d7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x7d7) = *(byte *)(unaff_A5 + -0x7d7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x7c7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x7c7) = *(byte *)(unaff_A5 + -0x7c7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x7b7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x7b7) = *(byte *)(unaff_A5 + -0x7b7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x7a7) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x7a7) = *(byte *)(unaff_A5 + -0x7a7) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x797) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x797) = *(byte *)(unaff_A5 + -0x797) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x787) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x787) = *(byte *)(unaff_A5 + -0x787) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x777) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x777) = *(byte *)(unaff_A5 + -0x777) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x767) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x767) = *(byte *)(unaff_A5 + -0x767) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x757) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x757) = *(byte *)(unaff_A5 + -0x757) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x747) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x747) = *(byte *)(unaff_A5 + -0x747) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x737) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x737) = *(byte *)(unaff_A5 + -0x737) | 0x80;
      }
      if ((*(byte *)(unaff_A5 + -0x727) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x727) = *(byte *)(unaff_A5 + -0x727) | 0x80;
      }
      DAT_00300001 = in_D0b;
      if ((*(byte *)(unaff_A5 + -0x717) & 2) == 0) {
        *(byte *)(unaff_A5 + -0x717) = *(byte *)(unaff_A5 + -0x717) | 0x80;
      }
    }
  }
  if (-1 < *(char *)(unaff_A5 + 0x1945)) {
    *(undefined1 *)(unaff_A5 + 0x192d) = DAT_00000000;
    iVar7 = unaff_A5 + -0x1716;
    puVar8 = (undefined2 *)(unaff_A5 + -0x706);
    bVar6 = *(byte *)(unaff_A5 + 0x191e) |
            *(byte *)(unaff_A5 + 0x191c) | *(byte *)(unaff_A5 + 0x191a);
    if (bVar6 != 0) {
      do {
        bVar4 = *(byte *)(iVar7 + 0xf);
        *(byte *)(iVar7 + 0xf) = bVar4 & 0x7f;
        if ((bVar4 & 0x80) != 0) {
          if (*(short *)(iVar7 + 0xc) == *(short *)(iVar7 + 10)) {
            if ((*(byte *)(iVar7 + 0xf) & 2) == 0) {
              bVar6 = candidate_59fe();
            }
            else {
              bVar6 = candidate_5978();
            }
          }
          else if ((*(byte *)(iVar7 + 0xf) & 2) == 0) {
            bVar6 = candidate_62ae();
          }
          else {
            bVar6 = candidate_5ea0();
          }
          *puVar8 = 0x8000;
          *(undefined1 *)(unaff_A5 + 0x192d) = 0xff;
        }
        iVar7 = iVar7 + 0x10;
        puVar8 = puVar8 + 0x10;
      } while (puVar8 != (undefined2 *)0x1098fa);
      *(undefined1 *)(unaff_A5 + 0x1945) = *(undefined1 *)(unaff_A5 + 0x192d);
      return bVar6;
    }
    do {
      bVar4 = *(byte *)(iVar7 + 0xf);
      *(byte *)(iVar7 + 0xf) = bVar4 & 0x7f;
      if ((bVar4 & 0x80) != 0) {
        if (*(short *)(iVar7 + 10) == *(short *)(iVar7 + 0xc)) {
          bVar6 = candidate_5978();
        }
        else {
          bVar6 = candidate_5ea0();
        }
        *puVar8 = 0x8000;
        *(undefined1 *)(unaff_A5 + 0x192d) = 0xff;
      }
      puVar8 = puVar8 + 0x10;
      iVar7 = iVar7 + 0x10;
    } while (puVar8 != (undefined2 *)0x1098fa);
    *(undefined1 *)(unaff_A5 + 0x1945) = *(undefined1 *)(unaff_A5 + 0x192d);
    return bVar6;
  }
  return in_D0b;
}
