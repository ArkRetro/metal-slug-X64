/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a99ee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a99ee(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    candidate_4256();
    *unaff_A6 = 0xa9a0e;
    uVar1 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar1) {
      *unaff_A6 = 0xa9a26;
    }
    return;
  }
  candidate_4256();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0x20000;
  *unaff_A6 = 0xa9a44;
  candidate_1456a();
  candidate_427a();
  candidate_a9a56();
  return;
}
