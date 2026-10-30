/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x076536.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_76536(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined8 uVar4;

  uVar4 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar4;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0xffe0;
  *(undefined2 *)(unaff_A6 + 0x1d) = 0;
  if (*(short *)((int)unaff_A6 + 0x7e) == 0) {
    candidate_120f8();
  }
  *unaff_A6 = 0x76574;
  candidate_1ac38();
  candidate_76914();
  if ((*(char *)((int)unaff_A6 + 0x61) == '\0') && (0x7f < *(byte *)((int)unaff_A6 + 0x12))) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1110;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1110;
  }
  candidate_427a();
  bVar3 = *(char *)((int)unaff_A6 + 0x12) != -1;
  if (*(char *)((int)unaff_A6 + 0x12) != -1) {
    *(char *)((int)unaff_A6 + 0x12) = *(char *)((int)unaff_A6 + 0x12) + '\x03';
    *(char *)((int)unaff_A6 + 0x13) = *(char *)((int)unaff_A6 + 0x13) + '\x03';
    bVar3 = *(byte *)((int)unaff_A6 + 0x12) < 2;
    if (*(byte *)((int)unaff_A6 + 0x12) < 3) {
      *(undefined1 *)((int)unaff_A6 + 0x12) = 0xff;
      *(undefined1 *)((int)unaff_A6 + 0x13) = 0xff;
      bVar3 = false;
    }
  }
  candidate_1945c();
  if (bVar3) {
    *(char *)(unaff_A6[3] + 0x60) = *(char *)(unaff_A6[3] + 0x60) + '\x01';
    candidate_4192();
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
