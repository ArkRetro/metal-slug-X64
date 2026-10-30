/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0237a0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_237a0(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x25) = *(undefined2 *)((int)unaff_A6 + 0x92);
  candidate_24b1c();
  candidate_40bc();
  *unaff_A6 = 0x237da;
  uVar2 = false;
  if (*(short *)(unaff_A6 + 0x25) != 0) {
    sVar1 = *(short *)(unaff_A6 + 0x25);
    uVar2 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x25) = sVar1 + -1;
  }
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar2) {
    uVar2 = 0;
    candidate_4256();
    candidate_24aec();
    if ((bool)uVar2) {
      uVar2 = 0;
      if (*(short *)(unaff_A6 + 0x25) == 0) {
        *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 2;
        *unaff_A6 = candidate_23892;
        uVar2 = 0;
      }
      else {
        candidate_24b1c();
      }
    }
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_23e96();
    *(byte *)(unaff_A6[3] + 0x8f) =
         *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
    *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 2;
    *unaff_A6 = candidate_24164;
    uVar2 = 0;
  }
  candidate_24c94();
  candidate_24a94();
  if ((bool)uVar2) {
    candidate_23e96();
    *(byte *)(unaff_A6[3] + 0x8f) =
         *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
    *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 2;
    *unaff_A6 = candidate_240ee;
  }
  return;
}
