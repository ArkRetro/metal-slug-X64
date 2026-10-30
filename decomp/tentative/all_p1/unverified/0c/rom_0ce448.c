/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ce448.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ce448(void)

{
  byte bVar1;
  ushort uVar2;
  short sVar3;
  short sVar4;
  int unaff_A6;

  candidate_1abc8();
  switch(*(byte *)(unaff_A6 + 0x80) & 7) {
  case 0:
    sVar4 = 0x10;
    break;
  case 1:
    sVar4 = 8;
    break;
  case 2:
    sVar4 = 1;
    break;
  default:
    return;
  case 4:
    bVar1 = *(byte *)(unaff_A6 + 0x74);
    *(char *)(unaff_A6 + 0x74) = *(char *)(unaff_A6 + 0x74) + '\b';
    *(int *)(unaff_A6 + 0x34) =
         (int)((uint)*(ushort *)((short)((ushort)bVar1 * 2) + 0x29fc) << 0x10) >> 8;
    *(undefined4 *)(unaff_A6 + 0x30) = 0;
    candidate_1ac38();
    if ((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0) {
      *(undefined2 *)(unaff_A6 + 0x70) = 0;
      *(undefined4 *)(unaff_A6 + 0x34) = 0;
    }
    return;
  case 5:
    bVar1 = *(byte *)(unaff_A6 + 0x74);
    *(char *)(unaff_A6 + 0x74) = *(char *)(unaff_A6 + 0x74) + '\b';
    *(int *)(unaff_A6 + 0x34) =
         (int)((uint)(ushort)(*(short *)((short)((ushort)bVar1 * 2) + 0x29fc) >> 1) << 0x10) >> 8;
    *(undefined4 *)(unaff_A6 + 0x30) = 0;
    candidate_1ac38();
    if ((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0) {
      *(undefined2 *)(unaff_A6 + 0x70) = 0;
      *(undefined4 *)(unaff_A6 + 0x34) = 0;
    }
    return;
  }
  sVar3 = (*(short *)(unaff_A6 + 0x92) - *(short *)(unaff_A6 + 0x40)) +
          (*(short *)(unaff_A6 + 0x9a) - *(short *)(unaff_A6 + 0x3c));
  if (sVar3 < 0) {
    sVar3 = -sVar3;
  }
  if (sVar4 < sVar3) {
    uVar2 = candidate_2eb4();
    sVar3 = (uVar2 & 0xff) * 2;
    sVar4 = *(short *)(sVar3 + 0x2a7c);
    *(int *)(unaff_A6 + 0x34) =
         (((int)*(short *)(sVar3 + 0x29fc) * (int)*(short *)(unaff_A6 + 0xa8) >> 8) << 0x10) >> 8;
    *(int *)(unaff_A6 + 0x30) = (((int)sVar4 * (int)*(short *)(unaff_A6 + 0xa8) >> 8) << 0x10) >> 8;
    candidate_1ac38();
    *(short *)(unaff_A6 + 0x82) = *(short *)(unaff_A6 + 0x82) + -1;
    if (0 < *(short *)(unaff_A6 + 0x82)) {
      return;
    }
  }
  *(undefined1 *)(unaff_A6 + 0x74) = 0;
  return;
}
