/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x080dd4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_80dd4(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  undefined8 uVar5;

  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1110;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1110;
  candidate_4256();
  candidate_29c6();
  uVar5 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar5;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *unaff_A6 = 0x80e36;
  *(undefined4 *)((int)unaff_A6 + 0x76) = unaff_A6[0xd];
  candidate_143dc();
  uVar3 = unaff_A6[0xc];
  uVar4 = uVar3 < (uint)((int)unaff_A6[0xc] >> 2);
  unaff_A6[0xc] = uVar3 - ((int)unaff_A6[0xc] >> 2);
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    if (*(char *)((int)unaff_A6 + 0x61) == '\0') {
      *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
      unaff_A6[0xc] = (int)unaff_A6[0xc] >> 2;
      unaff_A6[0xd] = -(*(int *)((int)unaff_A6 + 0x76) >> 1);
      uVar4 = 0;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
    }
    else {
      *unaff_A6 = 0x80c52;
      uVar4 = 0;
    }
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
