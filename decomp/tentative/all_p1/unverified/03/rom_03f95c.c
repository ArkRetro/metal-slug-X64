/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03f95c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3f95c(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;

  uVar1 = candidate_29c6();
  unaff_A6[0xd] = -((short)(uVar1 & 0x3fff) + 0x4000);
  *(undefined2 *)((int)unaff_A6 + 0x86) = 4;
  *(undefined2 *)(unaff_A6 + 0x1d) = 0xb;
  candidate_4256();
  *unaff_A6 = 0x3f996;
  *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
  if (*(short *)(unaff_A6 + 0x1d) < 1) {
    *unaff_A6 = candidate_3f9c6;
  }
  candidate_3fa74();
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  candidate_3fada();
  return;
}
