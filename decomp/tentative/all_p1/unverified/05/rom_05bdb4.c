/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05bdb4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5bdb4(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  *(int *)((int)unaff_A6 + 0x72) = -((int)unaff_A6[0xd] >> 4);
  candidate_4256();
  *unaff_A6 = 0x5bdd6;
  uVar2 = 0;
  candidate_5c050();
  candidate_5c21a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_5bdf6;
  }
  candidate_5c276();
  candidate_427a();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = 0x5bfc0;
  }
  uVar2 = false;
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_120f8();
    *unaff_A6 = 0x5bfc0;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
