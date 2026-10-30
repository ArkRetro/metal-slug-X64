/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a3d26.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a3d26(void)

{
  short sVar1;
  char cVar2;
  byte bVar3;
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(undefined2 *)((int)unaff_A6 + 0x86) = 600;
  candidate_1b04a();
  *(undefined4 *)((int)unaff_A6 + 0x7e) = extraout_A0;
  *unaff_A6 = 0xa3d3c;
  *(short *)((int)unaff_A6 + 0x86) = *(short *)((int)unaff_A6 + 0x86) + 1;
  if (*(char *)(unaff_A6 + 0x23) < (char)*(undefined2 *)((int)unaff_A6 + 0x86)) {
    *(undefined2 *)((int)unaff_A6 + 0x86) = 0;
    cVar2 = candidate_1b12e();
    bVar3 = (byte)(cVar2 + 8U) >> 3;
    if (bVar3 != *(byte *)((int)unaff_A6 + 0x8d)) {
      cVar2 = '\x01';
      if (bVar3 < *(byte *)((int)unaff_A6 + 0x8d)) {
        cVar2 = -1;
      }
      *(char *)((int)unaff_A6 + 0x8d) = cVar2 + *(char *)((int)unaff_A6 + 0x8d);
    }
    *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) & 0x1f;
    candidate_a4006();
  }
  *(short *)(unaff_A6 + 0x21) = *(short *)(unaff_A6 + 0x21) + -1;
  if (*(short *)(unaff_A6 + 0x21) < 1) {
    *unaff_A6 = 0xa3cb2;
  }
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_a3d86;
  }
  candidate_427a();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_a3d86;
  }
  uVar4 = 0;
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_a3d86;
    uVar4 = 0;
  }
  candidate_19504();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
