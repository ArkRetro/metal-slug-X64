/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x023892.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_23892(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x76) = *(undefined2 *)(*(int *)((int)unaff_A6 + 0x72) + 0x3c);
  *(undefined2 *)(unaff_A6 + 0x1e) = *(undefined2 *)(*(int *)((int)unaff_A6 + 0x72) + 0x40);
  candidate_24d06();
  *(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + -1;
  *unaff_A6 = 0x238d4;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = 0x23704;
    uVar1 = 0;
  }
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_23e96();
    *(byte *)(unaff_A6[3] + 0x8f) =
         *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
    *unaff_A6 = candidate_24164;
  }
  candidate_24c94();
  return;
}
