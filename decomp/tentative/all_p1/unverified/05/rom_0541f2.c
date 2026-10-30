/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0541f2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_541f2(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  char cVar4;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar5;

  candidate_12040();
  candidate_9ea8();
  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xfff1;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xfff1;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined1 *)(unaff_A6 + 0x1f) = 0;
  sVar3 = candidate_29f2();
  unaff_A6[0xc] = (int)(short)(sVar3 + 0x30) << 8;
  sVar3 = candidate_29f2();
  unaff_A6[0xd] = (int)(short)(sVar3 + 0x400) << 8;
  cVar4 = candidate_29c6();
  if (cVar4 < '\0') {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    unaff_A6[0xc] = -unaff_A6[0xc];
  }
  *unaff_A6 = 0x54274;
  unaff_A6[0xd] = unaff_A6[0xd] + -0x5000;
  candidate_1ac38();
  bVar5 = false;
  if (*(char *)(unaff_A6 + 0x1f) == '\0') {
    candidate_3c5c6();
    if (bVar5) {
      *(undefined1 *)(unaff_A6 + 0x1f) = 0xff;
    }
  }
  candidate_427a();
  if (*(short *)(unaff_A6 + 0x10) < 0x100) {
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
