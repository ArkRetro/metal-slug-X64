/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0772fa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_772fa(void)

{
  short sVar1;
  ushort uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_78dc8();
  candidate_78d96();
  candidate_1ac38();
  candidate_140c0();
  *(short *)(unaff_A6 + 0x1f) = *(short *)(unaff_A6 + 0x1f) + 1;
  uVar3 = *(ushort *)(unaff_A6 + 0x1f) < *(ushort *)((int)unaff_A6 + 0x7a);
  if ((short)*(ushort *)((int)unaff_A6 + 0x7a) <= (short)*(ushort *)(unaff_A6 + 0x1f)) {
    uVar2 = candidate_29c6();
    *(undefined2 *)(unaff_A6 + 0x1f) = *(undefined2 *)(candidate_e3efa + (short)((uVar2 & 3) * 2));
    uVar3 = 0;
    candidate_40bc();
  }
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar3 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    if (*(char *)((int)unaff_A6 + 0x9b) != '\x01') {
      *(undefined1 *)((int)unaff_A6 + 0x9b) = 1;
      candidate_49e8();
    }
    sVar1 = *(short *)((int)unaff_A6 + 0x76);
    uVar3 = sVar1 == 0;
    sVar1 = sVar1 + -1;
    *(short *)((int)unaff_A6 + 0x76) = sVar1;
    if (sVar1 == 0) {
      candidate_13f1e();
      *(undefined1 *)((int)unaff_A6 + 0x9b) = 2;
      *unaff_A6 = candidate_773ac;
      uVar3 = 0;
    }
  }
  candidate_78d46();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_77e50;
  }
  return;
}
