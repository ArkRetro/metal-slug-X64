/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04ad94.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4ad94(void)

{
  uint uVar1;
  undefined4 *unaff_A6;
  undefined8 uVar2;

  candidate_9f88();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x87d0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x87d0;
  candidate_9ea8();
  candidate_4256();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 4;
  uVar1 = candidate_29c6();
  if ((uVar1 & 1) == 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  candidate_29c6();
  uVar2 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar2;
  *(undefined1 *)((int)unaff_A6 + 0x12) = 0xf;
  *(undefined1 *)((int)unaff_A6 + 0x13) = 0xf;
  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x200;
  *unaff_A6 = candidate_4ae1c;
  return;
}
