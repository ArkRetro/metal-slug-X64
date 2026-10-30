/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0461d6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_461d6(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar3;

  candidate_464e2();
  if ((bool)in_CF) {
    candidate_4256();
    *unaff_A6 = 0x461f4;
    uVar3 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar3) {
      *unaff_A6 = 0x4620e;
    }
  }
  else if (*(char *)((int)unaff_A6 + 0x93) == '\0') {
    candidate_4256();
    *unaff_A6 = candidate_4631c;
    uVar3 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar3) {
      *unaff_A6 = 0x46126;
    }
  }
  else {
    candidate_4256();
    *unaff_A6 = 0x46230;
    uVar3 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_4624a;
    }
  }
  uVar3 = false;
  if (*(char *)(unaff_A6 + 0x24) == '\0') {
    candidate_44930();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_46370;
      uVar3 = 0;
    }
    candidate_4c24();
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_46336;
    uVar3 = 0;
  }
  candidate_1945c();
  if (!(bool)uVar3) {
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
