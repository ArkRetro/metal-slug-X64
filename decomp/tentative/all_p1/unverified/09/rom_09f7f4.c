/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09f7f4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9f7f4(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar1;

  candidate_a0532();
  if ((bool)in_CF) {
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xb;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    *unaff_A6 = 0x9f82a;
    candidate_a0356();
  }
  else if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_143dc();
  }
  else {
    candidate_1456a();
  }
  candidate_427a();
  if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
    *unaff_A6 = candidate_9f846;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = 0x9fca6;
  }
  bVar1 = false;
  candidate_1945c();
  if (bVar1) {
    *unaff_A6 = candidate_9fd60;
  }
  return;
}
