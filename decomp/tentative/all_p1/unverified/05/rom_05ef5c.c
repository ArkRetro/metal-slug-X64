/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05ef5c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5ef5c(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  candidate_1abc8();
  candidate_5fa5c();
  candidate_5fb26();
  candidate_5fb94();
  *unaff_A6 = 0x5ef74;
  uVar2 = 0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_143dc();
  }
  else {
    candidate_1456a();
  }
  candidate_4c24();
  if (((((bool)uVar2) && (*(char *)((int)unaff_A6 + 0x8b) != '\0')) &&
      ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0)) &&
     ((*(char *)((int)unaff_A6 + 0x89) == '\0' && (*(char *)((int)unaff_A6 + 0x8d) == '\0')))) {
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 0xff;
    unaff_A6[0x21] = unaff_A6[8];
    candidate_1b206();
    candidate_4256();
  }
  if (*(char *)(unaff_A6 + 0x22) == '\0') {
    unaff_A6[0x14] = 0x5eb62;
  }
  else {
    unaff_A6[0x14] = 0x5eb46;
  }
  bVar3 = false;
  candidate_427a();
  if ((bVar3) && (*(char *)((int)unaff_A6 + 0x8d) != '\0')) {
    candidate_4256();
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
  }
  candidate_5fa5c();
  if (*(char *)((int)unaff_A6 + 0x8d) == '\0') {
    sVar1 = *(short *)((int)unaff_A6 + 0x76) + *(short *)((int)unaff_A6 + 0x7a);
    if (*(int *)((int)unaff_A6 + 0x62) < 0) {
      if (sVar1 < *(short *)(unaff_A6 + 0xf)) goto LAB_0005f07a;
    }
    else if (*(short *)(unaff_A6 + 0xf) < sVar1) goto LAB_0005f07a;
    *(undefined1 *)((int)unaff_A6 + 0x8a) = 0xff;
    *unaff_A6 = 0x5f364;
  }
LAB_0005f07a:
  if (*(int *)((int)unaff_A6 + 0x62) < 0) {
    candidate_49e8();
  }
  else {
    candidate_49e8();
  }
  candidate_5fbee();
  candidate_5fd48();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  return;
}
