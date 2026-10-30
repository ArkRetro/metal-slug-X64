/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09fade.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9fade(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_120f8();
  *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xd;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(byte *)(unaff_A6 + 0x24) = *(byte *)(unaff_A6 + 0x24) | 0x20;
  if ((*(byte *)(unaff_A6 + 0x24) & 2) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x9fb2c;
  candidate_a0334();
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
