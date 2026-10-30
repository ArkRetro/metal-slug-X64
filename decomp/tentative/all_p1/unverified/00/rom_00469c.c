/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x00469c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_469c(void)

{
  ushort *puVar1;
  byte bVar2;
  char cVar3;
  ushort *puVar4;
  int iVar5;
  int *piVar6;
  int unaff_A5;

  puVar1 = *(ushort **)(unaff_A5 + -0x171a);
  *puVar1 = *puVar1 >> 1;
  puVar1[1] = puVar1[1] >> 1;
  puVar1[2] = puVar1[2] >> 1;
  puVar1[3] = puVar1[3] >> 1;
  puVar1[4] = puVar1[4] >> 1;
  puVar1[5] = puVar1[5] >> 1;
  puVar1[6] = puVar1[6] >> 1;
  puVar1[7] = puVar1[7] >> 1;
  puVar1[8] = puVar1[8] >> 1;
  puVar1[9] = puVar1[9] >> 1;
  puVar1[10] = puVar1[10] >> 1;
  puVar1[0xb] = puVar1[0xb] >> 1;
  puVar1[0xc] = puVar1[0xc] >> 1;
  puVar1[0xd] = puVar1[0xd] >> 1;
  puVar1[0xe] = puVar1[0xe] >> 1;
  puVar4 = puVar1 + 0x10;
  puVar1[0xf] = puVar1[0xf] >> 1;
  if ((ushort *)0x1068e5 < puVar4) {
    puVar4 = (ushort *)(unaff_A5 + -0x181a);
  }
  *(ushort **)(unaff_A5 + -0x171a) = puVar4;
  *(undefined1 *)(unaff_A5 + -0x181e) = *(undefined1 *)(unaff_A5 + -0x181d);
  *(undefined1 *)(unaff_A5 + -0x181d) = 0;
  *(undefined1 *)(unaff_A5 + -0x181c) = *(undefined1 *)(unaff_A5 + -0x181b);
  *(undefined1 *)(unaff_A5 + -0x181b) = 0;
  bVar2 = *(byte *)(unaff_A5 + -0x1c1e);
  *(byte *)(unaff_A5 + -0x1c1e) = bVar2 ^ 0x80;
  iVar5 = unaff_A5 + -0x1c1c;
  if ((bVar2 & 0x80) == 0) {
    iVar5 = unaff_A5 + -0x1a6a;
  }
  *(undefined2 *)(iVar5 + 0x100) = 0;
  *(undefined4 *)(iVar5 + 0x1ae) = 0;
  *(undefined4 *)(iVar5 + 0x1aa) = 0;
  *(undefined4 *)(iVar5 + 0x1a6) = 0;
  *(undefined4 *)(iVar5 + 0x1a2) = 0;
  *(undefined4 *)(iVar5 + 0x19e) = 0;
  *(undefined4 *)(iVar5 + 0x19a) = 0;
  *(undefined4 *)(iVar5 + 0x196) = 0;
  *(undefined4 *)(iVar5 + 0x192) = 0;
  *(undefined4 *)(iVar5 + 0x18e) = 0;
  *(undefined4 *)(iVar5 + 0x18a) = 0;
  *(undefined4 *)(iVar5 + 0x186) = 0;
  *(undefined4 *)(iVar5 + 0x182) = 0;
  *(undefined4 *)(iVar5 + 0x17e) = 0;
  *(undefined4 *)(iVar5 + 0x17a) = 0;
  *(undefined4 *)(iVar5 + 0x176) = 0;
  *(undefined4 *)(iVar5 + 0x172) = 0;
  *(undefined4 *)(iVar5 + 0x16e) = 0;
  *(undefined4 *)(iVar5 + 0x16a) = 0;
  *(undefined4 *)(iVar5 + 0x166) = 0;
  *(undefined4 *)(iVar5 + 0x162) = 0;
  *(undefined4 *)(iVar5 + 0x15e) = 0;
  *(undefined4 *)(iVar5 + 0x15a) = 0;
  *(undefined4 *)(iVar5 + 0x156) = 0;
  *(undefined4 *)(iVar5 + 0x152) = 0;
  *(undefined4 *)(iVar5 + 0x14e) = 0;
  *(undefined4 *)(iVar5 + 0x14a) = 0;
  *(undefined4 *)(iVar5 + 0x146) = 0;
  *(undefined4 *)(iVar5 + 0x142) = 0;
  *(undefined4 *)(iVar5 + 0x13e) = 0;
  *(undefined4 *)(iVar5 + 0x13a) = 0;
  *(undefined4 *)(iVar5 + 0x136) = 0;
  *(undefined4 *)(iVar5 + 0x132) = 0;
  *(undefined4 *)(iVar5 + 0x12e) = 0;
  *(undefined4 *)(iVar5 + 0x12a) = 0;
  *(undefined4 *)(iVar5 + 0x126) = 0;
  *(undefined4 *)(iVar5 + 0x122) = 0;
  *(undefined4 *)(iVar5 + 0x11e) = 0;
  *(undefined4 *)(iVar5 + 0x11a) = 0;
  *(undefined4 *)(iVar5 + 0x116) = 0;
  *(undefined4 *)(iVar5 + 0x112) = 0;
  *(undefined4 *)(iVar5 + 0x10e) = 0;
  *(undefined4 *)(iVar5 + 0x10a) = 0;
  *(undefined4 *)(iVar5 + 0x106) = 0;
  *(undefined4 *)(iVar5 + 0x102) = 0;
  piVar6 = (int *)(unaff_A5 + -0x18aa);
  cVar3 = '\0';
  do {
    if ((*piVar6 != 0) && (cVar3 != *(char *)(*piVar6 + 0x6b))) {
      *piVar6 = 0;
    }
    cVar3 = cVar3 + '\x01';
    piVar6 = piVar6 + 2;
  } while (piVar6 < (int *)0x1067d6);
  return;
}
