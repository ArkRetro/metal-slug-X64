/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02020c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2020c(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_1ac38();
  candidate_427a();
  candidate_140c0();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_202b2;
  }
  uVar2 = false;
  candidate_4c24();
  if ((bool)uVar2) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar2 = 0;
    }
    else {
      *unaff_A6 = candidate_202b2;
      uVar2 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_2010e;
  }
  return;
}
