/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05f780.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5f780(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  candidate_4192();
  candidate_4256();
  *unaff_A6 = 0x5f7ba;
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
  if (bVar3) {
    if (*(char *)((int)unaff_A6 + 0x8d) != '\0') {
      candidate_4256();
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
    }
    bVar3 = true;
  }
  else {
    bVar3 = false;
  }
  if ((bVar3) && (*(char *)((int)unaff_A6 + 0x8d) == '\0')) {
    *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + 1;
    uVar1 = candidate_260c8();
    if (*(ushort *)(unaff_A6 + 0x20) < uVar1) {
      *unaff_A6 = 0x5f5fc;
    }
    else {
      *(undefined1 *)(unaff_A6 + 0x23) = *(undefined1 *)(unaff_A6 + 0x22);
      *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x3c;
      *unaff_A6 = candidate_5f0a8;
    }
  }
  candidate_5fd48();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  return;
}
