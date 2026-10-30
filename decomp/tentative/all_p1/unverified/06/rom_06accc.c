/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06accc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6accc(void)

{
  char cVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_4256();
  *unaff_A6 = 0x6ace2;
  unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 4);
  candidate_143dc();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_6ad0c;
  }
  if (*(short *)((int)unaff_A6 + 0x82) != 0) {
    *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
  }
  if (*(char *)(unaff_A6 + 0x24) != '\0') {
    *(char *)(unaff_A6 + 0x24) = *(char *)(unaff_A6 + 0x24) + -1;
  }
  uVar2 = false;
  if (*(char *)((int)unaff_A6 + 0x91) != '\0') {
    cVar1 = *(char *)((int)unaff_A6 + 0x91);
    uVar2 = cVar1 == '\0';
    *(char *)((int)unaff_A6 + 0x91) = cVar1 + -1;
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_6af44;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = 0x6b27e;
  }
  return;
}
