/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a4f3a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a4f3a(void)

{
  short sVar1;
  byte bVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_1ad6e();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  candidate_4c1c();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 4;
  *(undefined1 *)((int)unaff_A6 + 0x89) = 2;
  bVar2 = candidate_29c6();
  *(byte *)(unaff_A6 + 0x23) = (bVar2 & 3) + 1;
  candidate_4256();
  unaff_A6[0xc] = 0x1800;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    unaff_A6[0xc] = -unaff_A6[0xc];
  }
  unaff_A6[0xd] = 0x11111;
  *(undefined2 *)((int)unaff_A6 + 0x86) = 0x1e;
  *unaff_A6 = 0xa4fde;
  sVar1 = *(short *)((int)unaff_A6 + 0x86) + -1;
  *(short *)((int)unaff_A6 + 0x86) = sVar1;
  if (sVar1 < 0) {
    candidate_4256();
    candidate_1b1dc();
    *unaff_A6 = 0xa5012;
    uVar3 = 0;
    candidate_1abc8();
    candidate_427a();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_a502c;
    }
  }
  else {
    candidate_1ac38();
    candidate_427a();
  }
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0x5f) & 0x80) != 0) {
    *unaff_A6 = FUN_000a52d6;
  }
  return;
}
