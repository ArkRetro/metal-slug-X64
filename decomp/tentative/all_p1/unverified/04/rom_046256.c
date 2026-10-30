/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x046256.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_46256(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  cVar3 = *(char *)((int)unaff_A6 + 0x91) + -1;
  *(char *)((int)unaff_A6 + 0x91) = cVar3;
  if (cVar3 < '\0') {
    candidate_4256();
    *unaff_A6 = 0x462bc;
    uVar4 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_462d6;
    }
  }
  else {
    candidate_4256();
    *unaff_A6 = 0x46274;
    uVar4 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_46256;
    }
    candidate_464b0();
    candidate_464ce();
    if ('\x14' < *(char *)((int)unaff_A6 + 0x92)) {
      *unaff_A6 = candidate_462d6;
    }
  }
  uVar4 = false;
  if (*(char *)(unaff_A6 + 0x24) == '\0') {
    candidate_44930();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_46370;
      uVar4 = 0;
    }
    candidate_4c24();
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_46336;
    uVar4 = 0;
  }
  candidate_1945c();
  if (!(bool)uVar4) {
    return;
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
