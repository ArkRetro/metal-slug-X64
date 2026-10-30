/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04611c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4611c(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(undefined1 *)((int)unaff_A6 + 0x93) = 1;
  uVar4 = 0;
  candidate_463f0();
  candidate_46436();
  candidate_4647c();
  if ((bool)uVar4) {
    candidate_4256();
    *unaff_A6 = 0x4618c;
    uVar4 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar4) {
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
