/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x059214.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_59214(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar3;
  bool bVar4;

  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_143dc();
  }
  else {
    candidate_1456a();
  }
  candidate_1945c();
  if ((bool)in_CF) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  uVar3 = 0;
  if ((*(short *)((int)unaff_A6 + 0x72) != 0) && (uVar3 = 0, *(short *)(unaff_A6 + 0x20) == 0)) {
    candidate_49e8();
  }
  candidate_4c24();
  if ((bool)uVar3) {
    candidate_59c4c();
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  bVar4 = false;
  candidate_427a();
  *(char *)((int)unaff_A6 + 0x8d) = -bVar4;
  if ((bVar4) && (*(char *)(unaff_A6 + 0x23) != '\0')) {
    candidate_4256();
    *(undefined1 *)(unaff_A6 + 0x23) = 0;
  }
  return;
}
