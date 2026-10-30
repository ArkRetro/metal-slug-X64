/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x046370.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_46370(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  unaff_A6[0xc] = 0x20000;
  if (*(short *)(unaff_A6 + 0xf) < 0xa0) {
    sVar3 = -0x1000;
  }
  else {
    sVar3 = 0x1000;
  }
  *(short *)((int)unaff_A6 + 0x8e) = sVar3;
  if (sVar3 <= *(short *)(unaff_A6 + 0xf)) {
    unaff_A6[0xc] = -unaff_A6[0xc];
  }
  *(undefined1 *)(unaff_A6 + 0x24) = 1;
  bVar4 = false;
  candidate_4647c();
  if (bVar4) {
    candidate_4256();
    *unaff_A6 = 0x4618c;
    uVar5 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar5) {
      *unaff_A6 = 0x46132;
    }
  }
  else {
    candidate_4256();
    *unaff_A6 = 0x46148;
    sVar3 = *(short *)(unaff_A6 + 0xf);
    func_0x000143e4();
    candidate_427a();
    if ((short)(sVar3 - *(short *)((int)unaff_A6 + 0x8e) ^
               *(short *)(unaff_A6 + 0xf) - *(short *)((int)unaff_A6 + 0x8e)) < 1) {
      *unaff_A6 = candidate_461d6;
    }
  }
  uVar5 = false;
  if (*(char *)(unaff_A6 + 0x24) == '\0') {
    candidate_44930();
    if ((bool)uVar5) {
      *unaff_A6 = candidate_46370;
      uVar5 = 0;
    }
    candidate_4c24();
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_46336;
    uVar5 = 0;
  }
  candidate_1945c();
  if (!(bool)uVar5) {
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
