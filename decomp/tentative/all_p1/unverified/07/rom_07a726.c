/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07a726.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7a726(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  uVar1 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x22) = uVar1;
  candidate_4256();
  *unaff_A6 = 0x7a74c;
  uVar2 = 0;
  candidate_1abc8();
  candidate_44f6();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = 0x7a680;
  }
  if (*(char *)(unaff_A6[3] + 0x61) == -1) {
    *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
    *unaff_A6 = 0x7aae8;
  }
  return;
}
