/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07e658.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7e658(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;

  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x86) = *(undefined2 *)((int)unaff_A6 + 0x6e);
  *unaff_A6 = 0x7e678;
  uVar4 = 0;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  }
  bVar5 = false;
  if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
    if (*(short *)((int)unaff_A6 + 0x86) == 0) {
      *unaff_A6 = candidate_7e6ca;
      bVar5 = false;
    }
    else {
      sVar3 = *(short *)((int)unaff_A6 + 0x86);
      bVar5 = sVar3 == 0;
      *(short *)((int)unaff_A6 + 0x86) = sVar3 + -1;
    }
  }
  candidate_1945c();
  if (bVar5) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
