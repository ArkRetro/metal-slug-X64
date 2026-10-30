/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0766e0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_766e0(void)

{
  ushort uVar1;
  undefined2 uVar2;
  ushort uVar3;
  int unaff_A6;

  uVar1 = *(ushort *)(*(int *)(unaff_A6 + 0xc) + 0x7e);
  uVar3 = uVar1;
  if (2 < (short)uVar1) {
    uVar3 = uVar1 - 3;
  }
  *(undefined2 *)(unaff_A6 + 0x2c) =
       *(undefined2 *)(*(int *)(unaff_A6 + 0xc) + (int)(short)(uVar3 * 2 + 0x76));
  candidate_1ac4c();
  candidate_1abb6();
  *(undefined4 *)(unaff_A6 + 0xa0) = *(undefined4 *)((uint)uVar1 * 0xc + 0xe3c80);
  *(undefined2 *)(unaff_A6 + 0xa4) = 0;
  *(undefined1 *)(unaff_A6 + 0xa6) = 0;
  uVar2 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x76) = uVar2;
  *(undefined1 *)(unaff_A6 + 0x85) = 0xff;
  *(undefined1 *)(unaff_A6 + 0x84) = 0;
  return;
}
