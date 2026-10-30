/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c8774.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c8774(void)

{
  short in_D0w;
  ushort uVar1;
  ushort *puVar2;
  int unaff_A5;

  *(short *)(unaff_A5 + 0x69d0) = in_D0w;
  if (in_D0w == 0) {
    uRam0010e9d0 = 0;
    candidate_c8084();
    func_0x000c81c0();
    return;
  }
  candidate_c8084();
  puVar2 = *(ushort **)(unaff_A5 + 0x6964);
  uVar1 = *puVar2;
  do {
    if (uVar1 < 0xc) {
                    /* WARNING: Switch is manually overridden */
      switch(uVar1) {
      case 0:
        uVar1 = puVar2[2];
        candidate_c880e();
        if (*(ushort *)(unaff_A5 + 0x69d0) <= uVar1) goto LAB_000c87d6;
        break;
      case 4:
        break;
      case 8:
        candidate_c880e();
LAB_000c87d6:
        candidate_c741a();
        candidate_c741a();
        candidate_c741a();
        candidate_c741a();
        *(undefined2 *)(unaff_A5 + 0x69d0) = 0;
        return;
      }
    }
    else {
      (*(code *)((short)uVar1 + 0xc844a))();
    }
    puVar2 = (ushort *)
             ((int)*(short *)(*(int *)(unaff_A5 + 0x6964) + 2) + *(int *)(unaff_A5 + 0x6964));
    *(ushort **)(unaff_A5 + 0x6964) = puVar2;
    uVar1 = *puVar2;
  } while( true );
}
