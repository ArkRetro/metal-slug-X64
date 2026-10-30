/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dc6aa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_dc6aa(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  ushort uVar4;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  sVar3 = candidate_1b1dc();
  unaff_A6[0xc] = *(undefined4 *)(sVar3 + 0xeb5b2);
  unaff_A6[0x1d] = *(undefined4 *)(sVar3 + 0xeb5b6);
  unaff_A6[0xd] = *(undefined4 *)(sVar3 + 0xeb5ba);
  uVar4 = candidate_29c6();
  unaff_A6[0xc] = (short)(uVar4 & 0x1ff) * 0x100 + unaff_A6[0xc];
  uVar4 = candidate_29c6();
  unaff_A6[0xd] = (short)(uVar4 & 0x1ff) * 0x100 + unaff_A6[0xd];
  if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
    unaff_A6[0xc] = -unaff_A6[0xc];
  }
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  candidate_29c6();
  candidate_4256();
  *unaff_A6 = 0xdc73e;
  candidate_143dc();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    unaff_A6[0xd] = (int)-unaff_A6[0xd] >> 1;
    candidate_40bc();
    candidate_1acf2();
    if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
      candidate_4126();
      iVar2 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar2 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  }
  candidate_427a();
  candidate_dc83c();
  return;
}
